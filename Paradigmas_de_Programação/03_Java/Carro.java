public class Carro extends Veiculo {
    private int numPortas;

    public int getNumPortas() {
        return this.numPortas;
    }

    public void setNumPortas(int numPortas) {
        if (numPortas > 0) {
            this.numPortas = numPortas;
        }
    }
}
