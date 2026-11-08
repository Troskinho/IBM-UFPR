import java.util.Scanner;

public class Contador {
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);
        int soma = 0;
        int num;
        System.out.print("Digite numeros para somar, -1 para parar: ");
        while (true) {
            num = teclado.nextInt();
            if (num == -1) {
                break;
            }
            soma += num;
        }
        System.out.println("A soma dos numeros digitados eh: " + soma);
        teclado.close();
    }
}   