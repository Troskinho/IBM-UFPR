public class Spinning extends Fitness implements Progressivel {
    private int carga;

    public Spinning(int duracao, int carga) {
        super("Spinnin", duracao);
        this.carga = carga;
    }

    @Override 
    public void executarTreino() {
        System.out.println("Pedalando a: " + carga);
    }

    @Override 
    public void aumentarProgressao() {
        carga++;
        System.out.println("Carga aumentada: " + carga);
    }

    @Override 
    public void descreverProgressao() {
        System.out.println("Aumenta-se a carga conforme melhoria");
    }
    
}
