public class Teste {

    public static void main(String[] args) {
        Endereco endereco = new Endereco();
        Data datanasc = new Data();
        Pessoa p1 = new Pessoa("Robin", "Programador", endereco, datanasc);
        p1.imprimir();
    }
}