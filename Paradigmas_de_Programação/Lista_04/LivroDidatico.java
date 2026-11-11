public class LivroDidatico extends Livro {
    private String disciplina;
    
    public LivroDidatico(String titulo, int ano, String editora, String disciplina) {
        super(titulo, ano, editora);
        this.disciplina = disciplina;
    }

    @Override 
    public void exibir() {
        super.exibir();;
        System.out.println("Disciplina: " + disciplina);
    }
}
