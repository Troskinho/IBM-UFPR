public abstract class Modalidade {
    protected String nome;
    protected String objetivo;

    public Modalidade(String nome, String objetivo) {
        this.nome = nome;
        this.objetivo = nome;
    }

    public void apresentar() {
        System.out.println("Modalidade: " + nome + " Objetivo: " + objetivo);
    }

    public abstract void regras();
}