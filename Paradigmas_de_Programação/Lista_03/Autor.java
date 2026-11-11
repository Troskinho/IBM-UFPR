public class Autor {
    private String nome;
    private String nacionalidade;

    public Autor() {
        this("Desconhecido", "Desconhecido");
    }

    public Autor(String nome) {
        this(nome, "Desconhecida");
    }

    public Autor(String nome, String nacionalidade) {
        this.nome = nome;
        this.nacionalidade = nacionalidade;
    }

    public String getNome() {
        return nome;
    }

    public String getNacionalidade() {
        return nacionalidade;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public void setNacionalidade(String nacionalidade) {
        this.nacionalidade = nacionalidade;
    }
}