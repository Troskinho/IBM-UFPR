import java.util.Scanner;

public class Vetor {
    public static void main(String[] args) {
        Scanner source = new Scanner(System.in);
        int[] valores = new int[5];
        int i, soma = 0;

        for (i = 0; i < valores.length; i++) {
            System.out.print("Digite o valor " + (i + 1) + ": ");
            valores[i] = source.nextInt();
            soma = soma + valores[i];
        }
        System.out.println("Soma: " + soma);
        source.close();
    }
}
