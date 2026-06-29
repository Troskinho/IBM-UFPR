#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//estrutura do nodo de uma arvore b
typedef struct nodo {
    int n;                      //numero atual de chaves armazenadas
    int* chaves;                //vetor dinamico que armazena as chaves
    struct nodo** filhos;        //vetor dinamico que armazena os filhos
    bool eh_folha;              //verificar se o nodo eh uma folha
} nodo;

//estrutura que retorna nodo e indice
typedef struct resultado {
    nodo* no_encontrado;
    int indice;
} resultado;

typedef struct arvoreB {
    nodo* raiz;
    int t;          //grau minimo da arvore
} arovreB;

void verificar_alocacao(void* ptr) {
    if (ptr == NULL) 
        exit(EXIT_FAILURE);
}

//recursiva, busca uma chave k a partir de um nodo x
resultado* busca_arvoreB(nodo* x, int k) {
    int i = 0;  //para ler o vetor a partir do indice 0

    //busca linear dentro do proprio no
    //avanca o indice enquanto nao chegar ao final do no ou 
    //enquanto a chave buscada for maior que as chaves do no
    while (i < x->n && k > x->chaves[i]) {
        i++;
    }

    //condicao que encontra a chave
    //se indice valido e a chave da posicao i eh a procurada
    if (i < x->n && k == x->chaves[i]) {
        resultado* encontrado = (resultado*)malloc(sizeof(resultado));
        verificar_alocacao(encontrado);
        encontrado->no_encontrado = x;
        encontrado->indice = i;
        return encontrado;
    }

    //condicao que chegou ao fundo da arvore
    //se nao encontrou a chave nesse nodo e nao possui filhos
    //a chave nao existe
    if (x->folha == true) {
        resultado* vazio = (resultado*)malloc(sizeof(resultado));
        verificar_alocacao(vazio);
        vazio->no_encontrado = NULL;
        vazio->indice = -1;
        return vazio;
    } 
    //recursao
    return busca_arvoreB(x->filhos[i], k); 
}

//alocar a memoria de um nodo na arovre B
nodo* alocaar_nodo(int t) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    verificar_alocacao(novo);

    //um nodo pode ter no maxio (2t -1) chaves e (2t) filhos
    novo->chaves = (int*)malloc(sizeof(int) * (2 * t - 1));
    verificar_alocacao(novo->chaves);
    novo->filhos = (nodo**)malloc(sizeof(nodo*) * (2 * t));
    verificar_alocacao(novo->filhos);

    novo->n = 0;
    novo->eh_folha = true;

    return novo;
}

void criar_arvoreB(arovreB T) {
    //alocar a memoria do nodo vazio
    nodo* x = alocaar_nodo(T->t);

    x->eh_folha = true;
    x->n = 0;

    T->raiz = x;
}
