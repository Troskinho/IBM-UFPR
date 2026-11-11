import java.util.Scanner;

public class Imc {
    public static void main(String[] args) {
        Scanner num = new Scanner(System.in);
        int acimaPeso = 0;

        System.out.print("Peso em kg (-1 para parar) : ");
        double peso = num.nextDouble();
        while (peso != -1) {
            System.out.print("Altura em metros: ");
            double altura = num.nextDouble();
            double imc = peso / (altura * altura);
            if (imc > 25) {
                acimaPeso++;
            }
            System.out.print("Peso em kg (-1 para parar): ");
            peso = num.nextDouble();
        }
        System.out.println("Pessoas acima do peso: " + acimaPeso);
        num.close();
    }
}