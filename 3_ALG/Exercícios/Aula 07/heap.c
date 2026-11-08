#include <stdio.h>
#include <stdlib.h>

int indice_pai(int i) {
    return (i - 1) / 2;
}

int indice_fe(int i) {
    return (2 * i) + 1;
}

int indice_fd(int i) {
    return (2 * i) + 2;
}

void trocar(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

//adaptada para vetores base 0
//c eh o vetor, i o indice do no  ser avaliado e n o tamanho atual da heap
void max_heapfy(int c[], int i, int n) {
    //encontrar os indices dos filhos
    int esquerdo = indice_fe(i);
    int direito = indice_fd(i);

    int maior = i;  //inicialmente o indice do pai eh o maior

    //compara com o filho esquerdo
    if (esquerdo < n && c[esquerdo] > c[i]) {
        maior = esquerdo;
    }
    //compara com o filho direito
    if (direito < n && c[direito] > c[maior]) {
        maior = direito;
    }

    //se o pai nao for o maior valor, essa chamada corrige e troca o pai com o maior filho
    if (maior != i) {
        trocar(&c[i], &c[maior]);
        //essa chamada recursiva corrige ja que o algum elemento desce n arvore
        //e isso pode novamente mexer na ordem da heap
        max_heapfy(c, maior, n);
    }
}

//construi uma max heap a partir de um vetor desordenado
void construir_max_heap(int v[], int n) {
    //encontra o indice do ultimo pai que possui um filho
    int ultimo_pai = (n / 2) - 1;

    //percorre a arovre de baixo para cima, do ultimo pai ate a raiz
    for (int i = ultimo_pai; i >= 0; i--) {
        max_heapfy(v, i, n);
        //aplica max heapfy para consertar a subarvore enraizada
    }
}
