// Pessoa.java (nova versão)
public class Pessoa {
    private String nome;
    private int idade;
    private Data nascimento = new Data();   // composição

    public Pessoa(String nome) {
        this.nome = nome;
    }

    public Data retornarDataNascimento() {
        return nascimento;
    }

    public String retornarNome() { return nome; }
    public int retornarIdade() { return idade; }

    // setters (prática)
    public void setNome(String nome) { this.nome = nome; }
    public void setNascimento(Data nascimento) { this.nascimento = nascimento; }

    public void calcularIdade(int diaAtual, int mesAtual, int anoAtual) {
        idade = anoAtual - nascimento.getAno();
        if (mesAtual < nascimento.getMes()
                || (mesAtual == nascimento.getMes() && diaAtual < nascimento.getDia())) {
            idade--;
        }
    }
}