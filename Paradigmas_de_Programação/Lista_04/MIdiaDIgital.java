public class MIdiaDIgital extends ItemBiblioteca {
    private String formato;

    public MIdiaDIgital(String titulo, int ano, String formato) {
        super(titulo, ano);
        this.formato = formato;
    }

    @Override 
    public void exibir() {
        super.exibir();;
        System.out.println("Formato: " + formato);
    }
}
