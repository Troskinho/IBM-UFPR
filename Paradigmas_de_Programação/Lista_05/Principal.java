public class Principal {
    public static void main(String[] args) {
        Spinning spinning = new Spinning(50, 5);

        spinning.apresentar();
        spinning.regras();
        spinning.exibirDuracao();
        spinning.executarTreino();
        spinning.aumentarProgressao();
        spinning.descreverProgressao();
    }
}
