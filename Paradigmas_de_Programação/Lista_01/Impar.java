import java.util.Scanner;

public class Impar {
    public static void main(String[] args) {
        Scanner num = new Scanner(System.in);
        System.out.print("Digite um número: ");
        int numero = num.nextInt();

        //operaror % devolve o resto da divisao 
        if (numero % 2 == 0) {
            System.out.println("O número é par.");
        } else {
            System.out.println("O número é ímpar.");
        }
        num.close();
    }
}
