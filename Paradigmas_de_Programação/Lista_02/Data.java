// Data.java
public class Data {
    private int dia, mes, ano;

    public void ajustarData(int dia, int mes, int ano) {
        this.dia = dia;
        this.mes = mes;
        this.ano = ano;
    }

    public void imprimirData() {
        System.out.println(dia + "/" + mes + "/" + ano);
    }

    // getters: necessários para a Pessoa calcular a idade no exercício 4
    public int getDia() { return dia; }
    public int getMes() { return mes; }
    public int getAno() { return ano; }

    // setters (prática): alteram um campo por vez
    public void setDia(int dia) { this.dia = dia; }
    public void setMes(int mes) { this.mes = mes; }
    public void setAno(int ano) { this.ano = ano; }
}