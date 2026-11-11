// Principal.java
public class Principal {
    public static void main(String[] args) {
        Pessoa[] pessoas = new Pessoa[2];

        pessoas[0] = new Pessoa("Grace Hopper");
        pessoas[0].retornarDataNascimento().ajustarData(9, 12, 1906);

        pessoas[1] = new Pessoa("Ada Lovelace");
        pessoas[1].retornarDataNascimento().ajustarData(10, 12, 1815);

        for (Pessoa p : pessoas) {
            p.calcularIdade(11, 11, 2026);
            System.out.println(p.retornarNome() + " - " + p.retornarIdade() + " anos");
            System.out.print("Nascimento: ");
            p.retornarDataNascimento().imprimirData();
        }
    }
}