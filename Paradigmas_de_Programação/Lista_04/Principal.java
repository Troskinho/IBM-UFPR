
public class Principal {
    public static void main(String[] args) {
        ItemBiblioteca[] itens = {
            new Livro("Estruturas de Dados", 2010, "Elsevier"),
            new Tese("Ensino de POO", 1998, "Prof. Silva"),
            new MIdiaDIgital("Aulas de Java", 2018, "mp3"),
            new LivroDidatico("Java: Como Programar", 2005, "Pearson", "CI1062")
        };

        ItemBiblioteca maisAntigo = itens[0];
        for (ItemBiblioteca item : itens) {
            if (item.getAno() < maisAntigo.getAno()) {
                maisAntigo = item;
            }
        }

        System.out.println("Item mais antigo:");
        maisAntigo.exibir();
    }
}