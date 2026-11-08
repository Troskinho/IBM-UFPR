"""
Automação para consolidar as "Specificity matrix" (distribuição de aminoácidos
nas posições P4..P4') de cada MEROPS ID listado numa página substrate_count.

Uso:
    python merops_scraper.py --action C --min-substrates 1 --out merops_distributions.csv

Requisitos:
    pip install requests beautifulsoup4 pandas lxml
"""

import argparse
import re
import time
import sys
from io import StringIO

import requests
import pandas as pd
from bs4 import BeautifulSoup

BASE = "https://www.ebi.ac.uk/merops"

HEADERS = {
    "User-Agent": (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36"
    ),
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
    "Accept-Language": "en-US,en;q=0.9,pt-BR;q=0.8",
}


def get_soup(url: str, session: requests.Session, debug_dump: str = None) -> BeautifulSoup:
    resp = session.get(url, headers=HEADERS, timeout=30)
    resp.raise_for_status()
    if debug_dump:
        with open(debug_dump, "w", encoding="utf-8") as f:
            f.write(resp.text)
        print(f"  [debug] status={resp.status_code} bytes={len(resp.text)} salvo em {debug_dump}", file=sys.stderr)
    return BeautifulSoup(resp.text, "lxml")


def list_merops_ids(action: str, session: requests.Session, min_substrates: int = 0):
    """Extrai (merops_id, nome, total_substrates) da página substrate_count."""
    url = f"{BASE}/cgi-bin/substrate_count?action={action}"
    soup = get_soup(url, session)

    rows = []
    for tr in soup.select("table tr"):
        cells = [td.get_text(strip=True) for td in tr.find_all("td")]
        if len(cells) < 3:
            continue
        merops_id, nome, total = cells[0], cells[1], cells[2]
        if not re.match(r"^[A-Z]\d{1,3}[A-Z]?\.\w+$", merops_id):
            continue
        try:
            total_i = int(total)
        except ValueError:
            continue
        if total_i >= min_substrates:
            rows.append((merops_id, nome, total_i))
    return rows


AREA_RE = re.compile(
    r'<area shape="rect" coords="([\d, ]+)"(?:\s+href="([^"]*)")?\s+title="([^"]*)">'
)
TREE_RANKS = ["superkingdom", "kingdom", "phylum", "class", "order", "family", "genus"]


def get_distribution_group(merops_id: str, session: requests.Session, debug_dump: str = None):
    """
    A frase-resumo ("Species containing peptidase X are all members of the
    group Y.") é desenhada dentro da imagem PNG da árvore, não existe como
    texto no HTML. Em vez de OCR, reconstruímos a mesma conclusão a partir
    dos <area title="rank: valor"> do mapa de imagem (<map name="peptidase_tree">),
    que contém a linhagem taxonômica completa usada para desenhar a árvore.

    Retorna um dict:
        {
            "species": [lista de organismos que contêm esse peptidase],
            "common_rank": "family" | "order" | ... | None,
            "common_value": "Caricaceae" | None,
            "sentence": frase equivalente à do site (nossa reconstrução)
        }
    """
    url = f"{BASE}/cgi-bin/pep_dist?mid={merops_id}"
    resp = session.get(url, headers=HEADERS, timeout=30)
    resp.raise_for_status()
    html = resp.text
    if debug_dump:
        with open(debug_dump, "w", encoding="utf-8") as f:
            f.write(html)

    areas = AREA_RE.findall(html)
    if not areas:
        return {"species": [], "common_rank": None, "common_value": None, "sentence": None}

    rows = {}
    order_list = []
    for coords, href, title in areas:
        try:
            x1, y1, x2, y2 = [int(v.strip()) for v in coords.split(",")]
        except ValueError:
            continue
        row_key = (y1, y2)
        if row_key not in rows:
            rows[row_key] = {}
            order_list.append(row_key)
        if ":" in title:
            rank, value = title.split(":", 1)
            rows[row_key][rank.strip()] = value.strip()
        else:
            rows[row_key]["species"] = title.strip()
            rows[row_key]["href"] = href

    # As linhas da árvore funcionam como um outline: um rank "vale" para
    # todas as linhas abaixo dele até aparecer um novo valor no mesmo rank.
    # Por isso precisamos propagar (forward-fill) na ordem vertical (y).
    order_list.sort(key=lambda k: k[0])
    last = {}
    filled_rows = []
    for key in order_list:
        r = rows[key]
        for rk in TREE_RANKS:
            if rk in r:
                last[rk] = r[rk]
            elif rk in last:
                r[rk] = last[rk]
        filled_rows.append(r)

    # Folhas = organismos que realmente têm esse peptidase (têm href pra speccards)
    species_rows = [r for r in filled_rows if r.get("href")]
    species_names = [r.get("species", "?") for r in species_rows]

    if not species_rows:
        return {"species": [], "common_rank": None, "common_value": None, "sentence": None}

    # Acha o rank mais específico em que todas as espécies concordam
    common_rank, common_value = None, None
    for rk in reversed(TREE_RANKS):  # do mais específico (genus) pro mais amplo
        values = {r.get(rk) for r in species_rows}
        if len(values) == 1 and None not in values:
            common_rank, common_value = rk, values.pop()
            break

    if common_value:
        sentence = f"Species containing peptidase {merops_id} are all members of the group {common_value}."
    else:
        sentence = f"Species containing peptidase {merops_id} are not confined to one particular group."

    return {
        "species": species_names,
        "common_rank": common_rank,
        "common_value": common_value,
        "sentence": sentence,
    }


def get_specificity_matrix(merops_id: str, session: requests.Session):
    """
    Busca a tabela 'Specificity matrix' na página pepsum?mid=<id>.
    Retorna um DataFrame long-format: MEROPS_ID, Amino_Acid, Position, Count
    ou None se o peptidase não tiver matriz (sem clivagens suficientes).
    """
    url = f"{BASE}/cgi-bin/pepsum?mid={merops_id}"
    soup = get_soup(url, session)

    # A tabela de especificidade tem cabeçalho "Amino acid" seguido de P4..P4'
    target_table = None
    for table in soup.find_all("table"):
        header_cells = [th.get_text(strip=True) for th in table.find_all("th")]
        if header_cells[:1] == ["Amino acid"] or "Amino acid" in header_cells:
            target_table = table
            break

    if target_table is None:
        return None

    df = pd.read_html(StringIO(str(target_table)))[0]
    df = df.rename(columns={df.columns[0]: "Amino_Acid"})
    long_df = df.melt(id_vars="Amino_Acid", var_name="Position", value_name="Count")
    long_df.insert(0, "MEROPS_ID", merops_id)
    return long_df


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--action", default="C", help="Código do clã/tipo catalítico (ex: C, S, M, A, T, G, N, U, P)")
    parser.add_argument("--min-substrates", type=int, default=1, help="Ignora IDs com menos que N substratos totais")
    parser.add_argument("--delay", type=float, default=1.0, help="Segundos de espera entre requisições (seja gentil com o servidor)")
    parser.add_argument("--out", default="merops_distributions.csv")
    parser.add_argument("--limit", type=int, default=None, help="Limita quantos IDs processar (útil para testar)")
    parser.add_argument(
        "--mode",
        choices=["specificity", "taxa-sentence", "both", "debug-one"],
        default="specificity",
        help=(
            "specificity: tabela P4..P4' (pepsum). "
            "taxa-sentence: frase da aba Distribution (pep_dist). "
            "both: gera os dois arquivos. "
            "debug-one: baixa o HTML bruto de --debug-id e mostra se a frase foi achada."
        ),
    )
    parser.add_argument("--debug-id", default="C01.001", help="ID usado no --mode debug-one")
    args = parser.parse_args()

    session = requests.Session()

    if args.mode == "debug-one":
        dump_path = f"debug_{args.debug_id}.html"
        info = get_distribution_group(args.debug_id, session, debug_dump=dump_path)
        print(f"Espécies encontradas: {info['species']}", file=sys.stderr)
        print(f"Grupo comum: {info['common_rank']} = {info['common_value']}", file=sys.stderr)
        print(f"Frase reconstruída: {info['sentence']}", file=sys.stderr)
        print(f"HTML bruto salvo em: {dump_path}", file=sys.stderr)
        return

    print(f"Buscando lista de MEROPS IDs para action={args.action} ...", file=sys.stderr)
    ids = list_merops_ids(args.action, session, min_substrates=args.min_substrates)
    print(f"{len(ids)} IDs com >= {args.min_substrates} substrato(s).", file=sys.stderr)

    if args.limit:
        ids = ids[: args.limit]

    all_frames = []
    sentence_rows = []
    for i, (merops_id, nome, total) in enumerate(ids, 1):
        print(f"[{i}/{len(ids)}] {merops_id} ({nome}) - {total} substratos", file=sys.stderr)

        if args.mode in ("specificity", "both"):
            try:
                df = get_specificity_matrix(merops_id, session)
                if df is not None:
                    all_frames.append(df)
            except Exception as e:
                print(f"  falhou (specificity): {e}", file=sys.stderr)
            time.sleep(args.delay)

        if args.mode in ("taxa-sentence", "both"):
            try:
                info = get_distribution_group(merops_id, session)
                sentence_rows.append(
                    {
                        "MEROPS_ID": merops_id,
                        "Nome": nome,
                        "Total_substratos": total,
                        "Total_especies": len(info["species"]),
                        "Especies": " | ".join(info["species"]),
                        "Grupo_comum_rank": info["common_rank"],
                        "Grupo_comum_valor": info["common_value"],
                        "Distribution_sentence": info["sentence"],
                    }
                )
            except Exception as e:
                print(f"  falhou (taxa-sentence): {e}", file=sys.stderr)
                sentence_rows.append(
                    {
                        "MEROPS_ID": merops_id,
                        "Nome": nome,
                        "Total_substratos": total,
                        "Especies": None,
                        "Grupo_comum_rank": None,
                        "Grupo_comum_valor": None,
                        "Distribution_sentence": None,
                    }
                )
            time.sleep(args.delay)

    if args.mode in ("specificity", "both") and all_frames:
        result = pd.concat(all_frames, ignore_index=True)
        out_spec = args.out if args.mode == "specificity" else args.out.replace(".csv", "_specificity.csv")
        result.to_csv(out_spec, index=False)
        print(f"Salvo em {out_spec} ({len(result)} linhas).", file=sys.stderr)

    if args.mode in ("taxa-sentence", "both") and sentence_rows:
        result_sent = pd.DataFrame(sentence_rows)
        out_sent = args.out if args.mode == "taxa-sentence" else args.out.replace(".csv", "_taxa_sentence.csv")
        result_sent.to_csv(out_sent, index=False)
        print(f"Salvo em {out_sent} ({len(result_sent)} linhas).", file=sys.stderr)


if __name__ == "__main__":
    main()