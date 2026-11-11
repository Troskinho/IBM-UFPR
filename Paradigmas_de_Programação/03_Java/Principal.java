public class Principal {
    public static void main(String[] args) {
        carro objeto1 = new carro();
        moto objeto2 = new moto();

        objeto1.ano = (int) (Math.random() * 20) + 2005;
        objeto1.modelo = "Gol";
        objeto1.cor = "Azul";

        objeto2.ano = (int) (Math.random() * 15) + 2010;
        objeto2.modelo = "CBR";
        objeto2.cor = "Vermelha";

        System.out.println("Carro: " + objeto1.modelo + " | Ano: " + objeto1.ano + " | Cor: " + objeto1.cor);
        System.out.println("Moto: " + objeto2.modelo + " | Ano: " + objeto2.ano + " | Cor: " + objeto2.cor);
    }
}

class carro {
    int ano;
    String modelo;
    String cor;
}

class moto {
    int ano;
    String modelo;
    String cor;
}
