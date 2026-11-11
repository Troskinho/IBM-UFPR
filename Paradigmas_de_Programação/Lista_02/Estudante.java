public class Estudante {
    private String nome;
    private double notaF;

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public double getNotaF() {
        return notaF;
    }

    public void setNotaF(double notaF) {
        this.notaF = notaF;
    }

    public void armazenar(String nome, double notaF) {
        this.nome = nome;
        this.notaF = notaF;
    }

    public void exibir() {
        System.out.println("Nome: "  + nome + " Nota Final: "  + notaF);
    }
}

