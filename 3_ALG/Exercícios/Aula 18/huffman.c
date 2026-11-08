#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef struct nodo {
    char caractere;
    unsigned int freq;
    struct nodo* fe;
    struct nodo* fd;
} nodo;
 
typedef struct min_heap {
    unsigned int tamanho;
    unsigned int capacidade;
    nodo** array;
} min_heap;
 
nodo* criar_nodo(char caractere, unsigned int freq) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    novo->caractere = caractere;
    novo->freq = freq;
    novo->fe = NULL;
    novo->fd = NULL;
    return novo;
}
 
//aloca uma min-heap com capacidade maxima
min_heap* criar_heap(unsigned int capacidade) {
    min_heap* Q = (min_heap*)malloc(sizeof(min_heap));
    Q->tamanho = 0;
    Q->capacidade = capacidade;
    Q->array = (nodo**)malloc(capacidade * sizeof(nodo*));
    return Q;
}
 
//troca dois ponteiros de nodo dentro do array da heap
static void trocar_nodos(nodo** a, nodo** b) {
    nodo* aux = *a;
    *a = *b;
    *b = aux;
}
 
//desce o elemento no indice i ate a posicao correta (min-heapify down)
static void heapify_down(min_heap* Q, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
 
    if (esq < (int)Q->tamanho && Q->array[esq]->freq < Q->array[menor]->freq)
        menor = esq;
    if (dir < (int)Q->tamanho && Q->array[dir]->freq < Q->array[menor]->freq)
        menor = dir;
 
    if (menor != i) {
        trocar_nodos(&Q->array[i], &Q->array[menor]);
        heapify_down(Q, menor);
    }
}
 
//sobe o elemento no indice i ate a posicao correta (heapify up)
static void heapify_up(min_heap* Q, int i) {
    int pai = (i - 1) / 2;
    while (i > 0 && Q->array[i]->freq < Q->array[pai]->freq) {
        trocar_nodos(&Q->array[i], &Q->array[pai]);
        i = pai;
        pai = (i - 1) / 2;
    }
}
 
//remove e retorna o nodo de menor frequencia (raiz da min-heap)
nodo* extrair_min(min_heap* Q) {
    if (Q->tamanho == 0) return NULL;
 
    nodo* minimo = Q->array[0];
 
    //move o ultimo elemento para a raiz e reduz o tamanho
    Q->array[0] = Q->array[Q->tamanho - 1];
    Q->tamanho--;
 
    //restaura a propriedade de heap descendo a raiz
    if (Q->tamanho > 0) {
        heapify_down(Q, 0);
    }
    return minimo;
}
 
//insere um nodo na min-heap
void inserir(min_heap* Q, nodo* no) {
    if (Q->tamanho == Q->capacidade) return; //heap cheia
 
    //insere no final e sobe ate a posicao correta
    Q->array[Q->tamanho] = no;
    Q->tamanho++;
    heapify_up(Q, Q->tamanho - 1);
}
 
//constroi a heap a partir de um array ja preenchido (nao ordenado)
//usa heapify_down a partir do ultimo no interno
void construir_heap(min_heap* Q) {
    int inicio = (int)Q->tamanho / 2 - 1;
    for (int i = inicio; i >= 0; i--) {
        heapify_down(Q, i);
    }
}
 
//algoritmo de huffman: recebe uma min-heap com os nodos folha
//e retorna a raiz da arvore de codigos construida
nodo* huffman(min_heap* Q) {
    //numero inicial de caracteres folhas na fila
    int n = Q->tamanho;
    //variaveis auxiliares
    nodo *z, *x, *y;
    //laco roda n-1 vezes mesclando nodos ate sobrar so a raiz
    for (int i = 1; i <= n - 1; i++) {
        //cria novo nodo interno (caractere '$' eh so um marcador)
        z = criar_nodo('$', 0);
        //extrai os nodos com menores frequencias da min-heap
        x = extrair_min(Q);
        y = extrair_min(Q);
        z->fe = x;
        z->fd = y;
        //a frequencia do novo nodo pai eh a soma das frequencias dos filhos
        z->freq = x->freq + y->freq;
        //insere a nova subarvore combinada de volta na heap
        inserir(Q, z);
    }
    //retorna a raiz, que eh o ultimo nodo restante
    return extrair_min(Q);
}
