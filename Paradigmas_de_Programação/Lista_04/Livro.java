public class Livro extends ItemBiblioteca {
    protected String editora;

    public Livro(String titulo, int ano, String editora) {
        super(titulo, ano);
        this.editora = editora;
    }

    @Override 
    public void exibir() {
        super.exibir();
        System.out.println("Editora: " + editora);
    }
}
