public class Principal {
    public static void main(String[] args) {
        Funcionario[] vetObjetos = new Funcionario[2];
        vetObjetos[0] = new Funcionario();
        vetObjetos[1] = new Funcionario();
        
        vetObjetos[0].cadastrar("Robin", 2021, 2500.00);
        vetObjetos[0].exibirDados();
        vetObjetos[1].cadastrar("Hulk", 2020, 1000.00); 
        vetObjetos[1].exibirDados();


        /* 
        Funcionario objeto1 = new Funcionario();
        Funcionario objeto2 = new Funcionario();

        objeto1.cadastrar("Robin", 2021, 2500.00);
        objeto1.exibirDados();
        System.out.println(objeto1.toString());

        objeto2.cadastrar("Hulk", 2020, 1000.00);
        objeto2.exibirDados();
        System.out.println(objeto2.toString());
        */
    }
}
