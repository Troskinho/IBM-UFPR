
import java.util.Comparator;
import java.util.Scanner;
import java.util.TreeSet;

public class Principal {
    public static void main(String[] args) {
        Scanner source = new Scanner(System.in);

        Comparator<Personagem> porDano = Comparator.comparingDouble(Personagem::calcularDano).reversed().thenComparing(Personagem::getNome);

        TreeSet<Personagem> personagens = new TreeSet<>(porDano);

        for (int i = 0; i < 6; i++) {
            System.out.print("Nome " + (i + 1));
            String nome = source.nextLine();
            System.out.print("Nivel: ");
            int nivel = Integer.parseInt(source.nextLine());
            System.out.print("Forca: ");
            double forca = Double.parseDouble(source.nextLine());

            switch (i % 3) {
                case 0 : personagens.add(new Guerreiro(nome, nivel, forca));
                case 1 : personagens.add(new Mago(nome, nivel, forca));
                default : personagens.add(new Arqueiro(nome, nivel, forca));
            }
        }
        for (Personagem p : personagens) {
            System.out.printf("%s - dano: %.2f%n", p.getNome(), p.calcularDano());
        }
        source.close();
    }
}