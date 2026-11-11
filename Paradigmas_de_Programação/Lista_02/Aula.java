import java.util.Scanner;

public class Aula {
    public static void main(String[] args) {
        Scanner num = new Scanner(System.in);
        Estudante[] turma = new Estudante[5];
        double soma = 0;

        for (int i = 0; i < turma.length; i++) {
            System.out.print("Nome do estudante " + (i + 1) + " ; ");
            String nome = num.nextLine();
            System.out.print("Nota final: ");
            double nota = Double.parseDouble(num.nextLine());

            turma[i] = new Estudante();
            turma[i].armazenar(nome, nota);
            soma += nota;
        }

        int exame = 0, reprovados = 0;
        for (Estudante e : turma) {
            if (e.getNotaF() < 40) {
                reprovados++;
            } else if (e.getNotaF() < 70) {
                exame++;
            }
        }
        System.out.println("Media da turma: " + (soma / turma.length));
        System.out.println("Foram para o exame: " + exame);
        System.out.println("Reprovados: " + reprovados);
        System.out.println("Aprovados: ");
        for (Estudante e : turma) {
            if (e.getNotaF() >= 70) {
                e.exibir();
            }
        }
        num.close();
    }

}