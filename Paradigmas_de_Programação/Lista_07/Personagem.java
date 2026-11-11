public abstract class Personagem {
    protected  String nome;
    protected  int nivel;
    protected  double forca;

    public Personagem(String nome, int nivel, double forca) {
        this.nome = nome;
        this.nivel = nivel;
        this.forca = forca;
    }

    public String getNome() {
        return nome;
    }

    public abstract double calcularDano();
}