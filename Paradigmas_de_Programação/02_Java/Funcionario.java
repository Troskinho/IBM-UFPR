public class Funcionario {
    private String nome;
    private int ano;
    private double salario;

    //definicao do metodo cadastrar
    public void cadastrar(String nome, int ano, double salario) {
        this.nome = nome;
        this.ano = ano;
        this.salario = salario;
    }

    //definicao do metodo para exibir os dados do funcionario
    public void exibirDados() {
        System.out.println("Nome: " + nome);
        System.out.println("Ano: " + ano);
        System.out.println("Salario: R$" + salario);
    }
}