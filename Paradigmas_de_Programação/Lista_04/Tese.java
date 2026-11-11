public class Tese extends ItemBiblioteca {
    private String orientador;

    public Tese(String titulo, int ano, String orientador) {
        super(titulo, ano);
        this.orientador = orientador;
    }

    @Override 
    public void exibir() {
        super.exibir();
        System.out.println("Orientador: " + orientador);

    }
}
