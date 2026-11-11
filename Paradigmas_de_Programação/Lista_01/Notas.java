import java.util.Scanner;

public class Notas {
    public static void main(String[] args) {
        Scanner num = new Scanner(System.in);
        double maior = Double.NEGATIVE_INFINITY;

        for (int aluno = 1; aluno <= 30; aluno++) {
            double maiorAluno = Double.NEGATIVE_INFINITY;
            for (int i = 1; i <= 4; i++) {
                System.out.print("Aluno " + aluno + " - nota " + i +": ");
                double nota = num.nextDouble();
                if (nota > maiorAluno) {
                    maiorAluno = nota;
                }
            }
            System.out.println("Maior nota do aluno " + aluno + ": " + maiorAluno);
            if (maiorAluno > maior) {
                maior = maiorAluno;
            }
        }

        System.out.println("Maior nota da turma: " + maior);
        num.close();;
    }
}