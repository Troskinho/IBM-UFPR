import java.util.Scanner;

public class Funcao {
    public static void main(String[] args) {
        Scanner num = new Scanner(System.in);
        System.out.print("Digite um número: ");
        double numero = num.nextDouble();
        double fx;

        if (numero < 0 && numero < 5) {
            fx = numero;
        } else if (numero >= 5 && numero < 10) {
            fx = 2 * numero + 1;
        } else if (numero >= 10) {
            fx = numero - 3;
        } else {
            System.out.print("Numero fora do dominio da função.");
            num.close();
            return;
        }

        System.out.println("O valor de f(x) é: " + fx);
        num.close();
    }

}