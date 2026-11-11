import java.util.Scanner;

public class Soma {
    public static void main(String[] args) {
        Scanner num = new Scanner(System.in);
        int soma = 0;
        System.out.print("Digite um número: (-1 para parar)");
        int numero = num.nextInt();

        //usamos o -1 como sentinela
        while (numero != -1) {
            soma += numero;
            System.out.print("Digite um número: (-1 para parar)");
            numero = num.nextInt();
        }
        System.out.println("A soma dos números digitados é: " + soma);
        num.close();
    }

}
