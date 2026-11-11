public class ImovelNovo extends Imovel {
    public ImovelNovo(Endereco endereco, double preco) {
        super(endereco, preco);
    }

    @Override 
    public double calcularValor() {
        return preco * 1.10;
    }
        
}
