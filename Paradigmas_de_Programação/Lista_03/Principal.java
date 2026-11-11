public class Principal {
    public static void main(String[] args) {
        Autor chapman = new Autor("Gary", "Americana");
        Livro l1 = new Livro("Exemplo", 2020, chapman, 3);

        l1.exibir();
    }
}
