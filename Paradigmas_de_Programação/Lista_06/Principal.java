import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class Principal {
    public static void main(String[] args) {
        Scanner source = new Scanner(System.in);
    
        System.out.print("Preço base dos imoveis novos: ");
        double precoNovo = source.nextDouble();
        System.out.print("Preço base dos imoveis velhos: ");
        double precoVelho = source.nextDouble();

        Imovel[] imovels = new Imovel[200];
        for (int i = 0; i < imovels.length; i++) {
            Endereco end = new Endereco("Rua" + i, i + 1, "centro", "curitiba", "pr");
            if (i % 2 == 0) {
                imovels[i] = new ImovelNovo(end, precoNovo);
            } else {
                imovels[i] = new ImovelVelho(end, precoVelho);
            }
        }
        List<Double> valores = new ArrayList<>();
        for (Imovel im : imovels) {
            double valor = im.calcularValor();
            valores.add(valor);
            System.out.printf("%s -> R$ %.2f%n", im.getEndereco(), valor);
        }
        System.out.printf("Primeiro imovel com 10% de desconto: R$ %.2f%n", imovels[0].calcularValor(10));
        source.close();
    }
}

