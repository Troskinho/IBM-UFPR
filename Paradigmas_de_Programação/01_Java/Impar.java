import java.util.Scanner;

public class Impar {
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);
        int num;
        System.out.print("Digite um numero: ");
        num = teclado.nextInt();
        if (num % 2 == 0) {
            System.out.println(num + " eh par");
        } else {
            System.out.println(num + " eh impar");
        }
        teclado.close();
    }
}