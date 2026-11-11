public class Guerreiro extends Personagem {
    public Guerreiro(String nome, int nivel, double forca) {
        super(nome, nivel, forca);
    }

    @Override 
    public double calcularDano() {
        return  forca * nivel * 1.2;
    }
}
