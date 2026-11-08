import java.util.Scanner;
//import eh usado para o compilador encontrar uma classe
//nesse caso, fazemos o import da classe scanner, que eh usada para ler dados do teclado

public class Principal {
    //a classe principal contem o metdo main que inicia o programa java
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        //essa declaracao cria um objeto input para ler os dados fornecidos pelo usuario
        int num;
        //a variavel num eh usada para armazenar um numero inteiro digitado
        System.out.print("Digite um numero: ");
        //intrucao acima exibe uma mensagem na tela para o usuario
        num = input.nextInt();
        //o metodo nextInt le o primeiro numero digitado pelo usuario e o atribui para a variavel num
        System.out.println("Numero digitado: " + num);
        //outra mensagem impressa na tela
        input.close();
        //o metodo close fecha o objeto input
    }
}