public class Veiculo{
    protected float preco;
    protected String marca;

    public float getPreco() {
        return this.preco;
    }

    public void setPreco(float preco) {
        if (preco > 0) {
            this.preco = preco;
        }
    }

    public String getMarca() {
        return this.marca;
    }

    public void setMarca(String marca) {
        this.marca = marca;
    }
}