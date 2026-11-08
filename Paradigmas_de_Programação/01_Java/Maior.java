import java.util.Scanner;

public class Maior {
    public static void main(String[] args){
        Scanner teclado = new Scanner(System.in);
        int num1, num2;
        System.out.print("Digite um numero: ");
        num1 = teclado.nextInt();
        System.out.print("Digite outro numero: ");
        num2 = teclado.nextInt();
        if(num1 < num2) {
            System.out.println(num1 + " " + num2);
        }
        else {
            System.out.println(num2 + " " + num1);
        }
        teclado.close();
    }
}