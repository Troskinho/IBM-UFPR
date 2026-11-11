public abstract class Fitness extends Modalidade {
    protected int duracao;

    public Fitness(String nome, int duracao)  {
        super(nome, "Melhorar condicionamento");
        this.duracao = duracao;
    }

    public void exibirDuracao() {
        System.out.println("Duracao: " + duracao);
    }

    public abstract void executarTreino();

    @Override 
    public void regras() {
        System.out.println("Progressão de peso");
    }
}
