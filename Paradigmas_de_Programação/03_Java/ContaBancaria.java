public class ContaBancaria {
    private String nomeT;
    private double saldo;

    public String getNomeT() {
        return this.nomeT;
    }
    
    public void setNomeT(String nomeT) {
        this.nomeT = nomeT;
    }
    
    public double getSaldo() {
        return this.saldo;
    }

    public void setSaldo(double saldo) {
        if (saldo >= 0) {
            this.saldo = saldo;
        }
    }


    //construtor padrao
    public ContaBancaria() {}

    //construtor 2
    public ContaBancaria(double saldo) {
        this(" ", saldo);
        //this.setSaldo(saldo);
    }

    //construtor 3
    public ContaBancaria(String nomeT) {
        this.setNomeT(nomeT);
    }

    //construtor 4
    public ContaBancaria(String nomeT, double saldo) {
        this.setNomeT(nomeT);
        this.setSaldo(saldo);
    }
}
