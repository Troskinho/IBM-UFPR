#include "gbv.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

static char buffer1[BUFFER_SIZE]; //buffer global para leitura/escrita
static char buffer2[BUFFER_SIZE]; //segundo buffer global para operações 

//Nome da biblioteca ativa — gravado em gbv_open, usado por gbv_view
static char s_archive[512];

//estrutura interna do superbloco 
typedef struct {
    int count;                  //numero de documentos
    long offset_diretorio;;     //offset do diretorio no arquivo
} superbloco;

//le o superbloco do inicio do arquivo
static int ler_suplerbloco(FILE *file, superbloco *sb) {
    rewind(file);
    if (fread(sb, sizeof(superbloco), 1, file) != 1) {
        return -1; //erro ao ler o superbloco
    }
    return 0;
}

//grava o superbloco no inicio do arquivo
static int escrever_superbloco(FILE *file, const superbloco *sb) {
    rewind(file);
    if (fwrite(sb, sizeof(superbloco), 1, file) != 1) {
        return -1;//erro ao escrever o superbloco
    }
    return 0;
}

static int salvar_biblioteca(FILE *file, Library *lib, long offset_diretorio) { 
    //vai ao inicio do diretorio
    if (fseek(file, offset_diretorio, SEEK_SET) != 0) {
        return -1;
    }

    for (int i= 0; i < lib->count; i++) {
        if (fwrite(&lib->docs[i], sizeof(Document), 1, file) != 1) {
            return -1; 
        }

        superbloco sb;
        sb.count = lib->count;
        sb.offset_diretorio = offset_diretorio;
        if (escrever_superbloco(file, &sb) != 0) {
            return -1;  
        }
    }
    
    return 0;
}

int gbv_create(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return -1;

    superbloco sb;
    sb.count = 0;
    sb.offset_diretorio = sizeof(superbloco); //diretorio logo apos o superbloco
    
    if (fwrite(&sb, sizeof(superbloco), 1, file) != 1) {
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;   
}

int gbv_open(Library *lib, const char *filename) {
    lib->docs = NULL;
    lib->count = 0;

    strncpy(s_archive, filename, sizeof(s_archive) - 1);
    s_archive[sizeof(s_archive) - 1] = '\0';

    FILE *file = fopen(filename, "rb");
    if (!file) {    
        return gbv_create(filename); //cria nova biblioteca se o arquivo nao existir
    }

    superbloco sb;
    if (ler_superbloco(file, &sb) != 0) {
        fclose(file);
        return -1; 
    }

    lib->count = sb.count;

    if (sb.count == 0) {
        fclose(file);
        return 0; //biblioteca vazia
    }

    lib->docs = malloc(sb.count * sizeof(Document));
    if (!lib->docs) {
        fclose(file);
        return -1; //erro de alocacao 
    }

    if (fseek(file, sb.offset_diretorio, SEEK_SET) != 0) {
        free(lib->docs);
        fclose(file);
        return -1; //erro ao posicionar no diretorio
    }

    for (int i = 0; i < sb.count; i++) {
        if (fread(&lib->docs[i], sizeof(Document), 1, file) != 1) {
            free(lib->docs);
            fclose(file);
            return -1; //erro ao ler o diretorio
        }
    }

    fclose(file);
    return 0;
}

int gbv_add(Library *lib, const char *archive, const char *docname){
    FILE *src = fopen(docname, "rb");
    if (!src) {
        printf("Erro: não foi possível abrir '%s'\n", docname);
        return -1;
    }

    fseek(src, 0, SEEK_END);
    long doc_size = ftell(src);
    rewind(src);

    FILE *lib_f = fopen(archive, "r+b");

    if (!lib_f) {
        fclose(src);
        return -1;
    }

    superbloco sb;

    if (ler_superbloco(lib_f, &sb) != 0) {
        fclose(src);
        fclose(lib_f);
        return -1;
    }

    const char *doc_basename = strrchr(docname, '/');
    doc_basename = doc_basename ? doc_basename + 1 : docname;

    int existing_idx = -1;
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, doc_basename) == 0){
            existing_idx = i;
            break;      
        }
    }
    long new_offset = sb.offset_diretorio;

    if (existing_idx >= 0) {
        // O documento existente é descartado (remoção lógica):
        // simplesmente não o incluiremos no novo diretório.
        // Seus bytes ficam no arquivo mas sem entrada no diretório.
        // Copiamos os dados das OUTRAS entradas mantendo os offsets,
        // e o novo documento entra no lugar do diretório.
    }
    

    if (fseek(lib_f, sb.offset_diretorio, SEEK_SET) != 0) {
        fclose(src);
        fclose(lib_f);
        return -1;
    }

    long bytes_escritos = 0;
    size_t bytes_lidos;
    while ((bytes_lidos = fread(buffer1, 1, BUFFER_SIZE, src)) > 0) {
        if (fwrite(buffer1, 1, bytes_lidos, lib_f) != bytes_lidos) {
            fclose(src);
            fclose(lib_f);
            return -1;
        }
        bytes_escritos += bytes_lidos;
    }

    fclose(src);

    long novo_offset_diretorio = new_offset + bytes_escritos;

    Document novo_doc;
    memset(&novo_doc, 0, sizeof(Document));
    strncpy(novo_doc.name, doc_basename, MAX_NAME - 1);
    novo_doc.size = doc_size;
    novo_doc.date = time(NULL);
    novo_doc.offset = new_offset;

    if (existing_idx >= 0) {
        lib->docs[existing_idx] = novo_doc;
    } else {
        Document *tmp = realloc(lib->docs, (lib->count + 1) * sizeof(Document));
        if (!tmp) {
            fclose(lib_f);
            return -1;
        }
        lib->docs = tmp;
        lib->docs[lib->count] = novo_doc;
        lib->count++;
    }

    if (salvar_biblioteca(lib_f, lib, novo_offset_diretorio) != 0) {
        fclose(lib_f);
        return -1;
    }

    fclose(lib_f);
    printf("Documento '%s' adicionado com sucesso.\n", doc_basename);
    return 0;
}

int gbv_remove(Library *lib, const char *docname){
    int idx = -1;

    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, docname) == 0) {
            idx = i;
            break;
        }
    }

    if (idx < 0) {
        printf("Documento '%s' não encontrado.\n", docname);
        return -1;
    }

    for (int i = idx; i < lib->count - 1; i++) {
        lib->docs[i] = lib->docs[i + 1];
    }
    lib->count--;

    if (lib->count == 0) {
        free(lib->docs);
        lib->docs = NULL;
    } else {
        Document *tmp = realloc(lib->docs, lib->count * sizeof(Document));
        if (tmp) lib ->docs = tmp;
    }
    
    printf("Documento '%s' removido.\n", docname);
    
    FILE *file = fopen(s_archive, "r+b");;
    if (!file) return -1;

    superbloco sb;
    if (ler_superbloco(file, &sb) != 0) {
        fclose(file);
        return -1;
    }

    if (fseek(file, sb.offset_diretorio, SEEK_SET) != 0) {
        fclose(file);
        return -1;
    }

    for (int i = 0; i < lib->count; i++) {
        fwrite(&lib->docs[i], sizeof(Document), 1, file);
    }

    sb.count = lib->count;
    escrever_superbloco(file, &sb);
    fclose(file);

    return 0;
}

int gbv_list(const Library *lib) {
    if (lib->count == 0) {
        printf("BIblioteca vazia.\n");
        return 0;
    }

    printf("%-30% %10  %-20s  %s\n", "Nome", "Tamanho", "Data de Inserção", "Offset");
    printf("%-30s %10s  %-20s  %s\n", 
              "------------------------------", "----------",
              "--------------------", "------");
    for (int i = 0; i < lib->count; i++) {
        char date_str[32];
        format_date(lib->docs[i].date, date_str, sizeof(date_str));
        printf("%-30s %10ld  %-20s  %ld\n",
               lib->docs[i].name,
               lib->docs[i].size,
               date_str,
               lib->docs[i].offset);    
    }

    return 0;
}

int gbv_view(const Library *lib, const char *docname) {
    int idx = -1;
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, docname) == 0) {
            idx = i;
            break;
        }
    }

    if (idx < 0) {
        print ("Documento '%s' não encontrado.\n", docname);
        return -1;
    }

    FILE *f = fopen(s_archive, "rb");
    if (!f) {
        printf("Erro ao abrir a biblioteca.\n");
        return -1;
    }

    Document *doc = &lib->docs[idx];
    long total_bytes = doc->size;
    long total_blocos = (total_bytes + BUFFER_SIZE - 1) / BUFFER_SIZE;
    long atual = 0;

    while (1) {
        long bloco_offset = doc->offset + atual * BUFFER_SIZE;
        if (fseek(f, bloco_offset, SEEK_SET) != 0) break;

        size_t para_ler = BUFFER_SIZE;
        long restante = total_bytes - atual * BUFFER_SIZE;
        if (restante < (long)para_ler) para_ler = (size_t)restante;

        size_t n = fread(buffer1, 1, para_ler, f);
        if (n == 0) break;

        print ('\n--- Bloco %ld/%ld ---\n', atual + 1, total_blocos);
        fwrite(buffer1, 1, n, stdout);
        printf("\n");

        printf("[n=próximo | p=anterior | q=sair] > ");
        fflush(stdout);

        char cmd[8];
        if (!fgets(cmd, sizeof(cmd), stdin)) break;

        if (cmd[0] == 'q') break;
        else if (cmd[0] == 'n') {
            if (atual < total_blocos - 1) atual++;
            else printf("(último bloco)\n");
        } else if (cmd[0] == 'p') {
            if (atual > 0) atual --;
            else printf("(primeiro bloco)\n");
        }
    }

    fclose(f);
    return 0;
}

//funções para usar no qsort

static int cmp_nome(const void *a, const void *b) {
    return strcmp(((Document *)a)->name, ((Document *)b)->name);
}

static int cmp_data(const void *a, const void *b) {
    time_t ta = ((Document *)a)->date;
    time_t tb = ((Document *)b)->date;
    return (ta > tb) - (ta < tb); //retorna positivo se a > b, negativo se a < b, zero se iguais
}

static int cmp_tamanho(const void *a, const void *b) {
    long sa = ((Document *)a)->size;
    long sb = ((Document *)b)->size;
    return (sa > sb) - (sa < sb); //retorna positivo se a > b, negativo se a < b, zero se iguais
}

int gbv_order(Library *lib, const char *archive, const char *criteria) {
    if (lib->count == 0) {
        printf("Biblioteca vazia.\n");
        return 0;
    }

    int (*cmp)(const void *, const void*) = NULL;
    if (strcmp(criteria, "nome") == 0)
        cmp = cmp_nome;
    else if (strcmp(criteria, "data") == 0)
        cmp = cmp_data;
    else if (strcmp(criteria, "tamanho") == 0)
        cmp = cmp_tamanho;
    else {
        printf("Critério inválido. Use: nome | data | tamanho\n");
        return -1;
    }

    qsort(lib->docs, lib->count, sizeof(Document), cmp); //ordena diretorio na memoria

    char tmp_name[MAX_NAME + 8];
    snprintf(tmp_name, sizeof(tmp_name), "%s.tmp", archive);

    FILE *orig = fopen(archive, "rb");
    FILE *tmp = fopen(tmp_name, "wb");

    if (!orig || !tmp) {
        if (orig)
            fclose(orig);
        if (tmp)
            fclose(tmp);
        return -1;
    }

    superbloco sb_aux;
    sb_aux.count = lib->count;
    sb_aux.offset_diretorio = sizeof(superbloco); //diretorio logo
    fwrite(&sb_aux, sizeof(superbloco), 1, tmp);

    long novo_data_offset = sizeof(superbloco);

    for (int i = 0; i < lib->count; i++) {

        long offset_antigo = novo_data_offset;

        if (fseek(orig, lib->docs[i].offset, SEEK_SET) != 0) {
            fclose(orig);
            fclose(tmp);
            remove(tmp_name);
            return -1;
        }

        long restante = lib->docs[i].size;

        while (restante > 0) {
            size_t para_ler = (restante > BUFFER_SIZE) ? BUFFER_SIZE : (size_t)restante;
            size_t n = fread(buffer1, 1, para_ler, orig);
            if (n == 0) break;
            memcpy(buffer2, buffer1, n);
            if (fwrite(buffer2, 1, n, tmp) != n) {
                fclose(orig);
                fclose(tmp);
                remove(tmp_name);
                return -1;
            }

            restante -= n;
            novo_data_offset += n;
        }

        lib->docs[i].offset = offset_antigo; //atualiza offset para nova posição
    }

    fclose(orig);

    long dir_offset = novo_data_offset;

    for (int i = 0; i < lib->count; i++) {
        fwrite(&lib->docs[i], sizeof(Document), 1, tmp);
    }

    superbloco sb_final;
    sb_final.count = lib->count;
    sb_final.offset_diretorio = dir_offset;;
    rewind(tmp);
    fwrite(&sb_final, sizeof(superbloco), 1, tmp);

    fclose(tmp);

    remove(archive);
    rename(tmp_name, archive);

    printf("Biblioteca reordenada por '%s'.\n", criteria);
    return 0;
}
