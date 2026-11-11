public class ItemBiblioteca {
    protected String titulo;
    protected int ano;

    public ItemBiblioteca(String titulo, int ano) {
        this.titulo = titulo;
        this.ano = ano;
    }

    public int getAno() {
        return ano;
    }

    public void exibir() {
        System.out.println("Titulo: " + titulo + "Ano: " + ano);
    }
}