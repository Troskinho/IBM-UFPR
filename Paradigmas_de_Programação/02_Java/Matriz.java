public class Matriz {
    public static void main(String[] args) {

        int [] [] m = {{1,2}, {3,4}, {5,6}};
        int linha, coluna;

        for(linha = 0; linha < m.length; linha++) {
            for(coluna = 0; coluna < m[linha].length; coluna++) {
                System.out.println(m[linha][coluna] + " ");
                
            }
            System.out.print("\n");
        }
    }
}
