public class ImovelVelho extends Imovel {
    public ImovelVelho(Endereco endereco, double preco) {
        super(endereco, preco);
    }

    @Override 
    public double calcularValor() {
        return preco * 0.85;
    }
}