public abstract class Imovel {
    protected Endereco endereco;
    protected double preco;

    public Imovel(Endereco endereco, double preco) {
        this.endereco = endereco;
        this.preco = preco;
    }

    public abstract double calcularValor();

    public double calcularValor(double percentualD) {
        return  calcularValor() * (1 - percentualD / 100);
    }

    public Endereco getEndereco() {
        return endereco;
    }
}
