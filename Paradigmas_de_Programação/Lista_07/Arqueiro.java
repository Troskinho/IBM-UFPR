public class Arqueiro extends Personagem {
    public Arqueiro(String nome, int nivel, double forca) {
        super(nome, nivel, forca);
    }

    @Override 
    public double calcularDano() {
        return forca * nivel + 15;
    }
}
