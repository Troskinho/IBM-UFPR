public class Mago extends Personagem {
    public Mago(String nome, int nivel, double forca) {
        super(nome, nivel, forca);
    }

    @Override 
    public double calcularDano() {
        return forca * nivel * 1.5 - 10;
    }
    
}
