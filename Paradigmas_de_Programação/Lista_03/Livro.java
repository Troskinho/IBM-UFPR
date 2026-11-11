public class Livro {
    private String titulo;
    private int ano;
    private Autor autor;
    private int edicao;

    public Livro() {
        this("Sem titulo", 0, new Autor(), 1);
    }

    public Livro(String titulo, int ano, Autor autor) {
        this(titulo, ano, autor, 1);
    }

    public Livro(String titulo, int ano, Autor autor, int edicao) {
        this.titulo = titulo;
        this.ano = ano;
        this.autor = autor;
        this.edicao = edicao;
    }

    public void exibir() {
        System.out.println("Titulo: " + titulo);
        System.out.println("Ano: " + ano);
        System.out.println("Autor: " + autor);
        System.out.println("Edicao: " + edicao);
    }
}
