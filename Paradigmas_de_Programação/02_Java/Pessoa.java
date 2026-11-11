public class Pessoa {
    private String nome;
    private String ocup;
    private Endereco endereco;
    private Data dataNasc;

    public String getNome() {
        return nome;
    }  

    public void setNome(String nome) {
        this.nome = nome;
    }

    public String getOcup() {
        return ocup;
    }

    public void setOcup(String ocup) {
        this.ocup = ocup;
    }

    public Endereco getEndereco() {
        return endereco;
    }

    public void setEndereco(Endereco endereco) {
        this.endereco = endereco;
    }   

    public Data getDataNasc() {
        return dataNasc;
    }   

    public void setDataNasc(Data dataNasc) {
        this.dataNasc = dataNasc;
    }

    //construtor padrao e construtor completo
    public Pessoa(){}
    public Pessoa(String nome, String ocup, Endereco endereco, Data dataNasc) {
        this.setNome(nome);
        this.setOcup(ocup);
        this.setEndereco(endereco);
        this.setDataNasc(dataNasc);
    }

    public void imprimir() {
        System.out.println("Nome: " + this.getNome());
        System.out.println("Ocupacao: " + this.getOcup());
        System.out.println("Endereco: " + this.getEndereco().getRua() + ", " + this.getEndereco().getNumero() + ", " + this.getEndereco().getBairro() + ", " + this.getEndereco().getCidade() + ", " + this.getEndereco().getEstado() + ", " + this.getEndereco().getCep());
        System.out.println("Data de Nascimento: " + this.getDataNasc().getDia() + "/" + this.getDataNasc().getMes() + "/" + this.getDataNasc().getAno());
    }
}