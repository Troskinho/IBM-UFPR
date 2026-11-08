import java.util.Scanner;

public class Tabuada {
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);
        int num, cont, mult;
        System.out.print("Digite um numero: ");
        num = teclado.nextInt();
        //laco de repeticao usando while
        /*
        cont = 1;
        while (cont <= 10) {
            mult = num * cont;
            System.out.println(num + "*" + cont + " = " + mult);
            cont++;
        }
        */
       //laco de repeticao usando for
       for (cont = 1; cont <= 10; cont++) {
            mult = num * cont;
            System.out.println(num + "*" + cont + " = " + mult);
       }
        teclado.close();
    }   
}