#include <stdio.h>
#include <stdlib.h>


//forma de representar uma arvore binaria
typedef struct nodo {
    int chave;
    struct nodo* fe;
    struct nodo* fd;
} nodo;

//funcao que calcula a altura de uma arvore binaria
//calcula de forma recursiva
int calcular_altura(nodo* raiz) {

    int alt_esq, alt_dir;

    if (raiz == NULL) 
        return -1;
    
    alt_esq = calcular_altura(raiz->fe);
    alt_dir = calcular_altura(raiz->fd);

    if (alt_esq > alt_dir)
        return alt_esq + 1;
    else
        return alt_dir + 1;
}

//travessias de forma recursiva

//visita a arvore em pre ordem
void pre_ordem(nodo* raiz) {

    if (nodo != NULL) {
        printf("%d ", raiz->chave);
        pre_ordem(raiz->fe);
        pre_ordem(raiz->fd);
    }
}

//visita a arvore em ordem
void em_ordem(nodo* raiz) {

    if (nodo != NULL) {
        em_ordem(raiz->fe);
        printf("%d ", raiz->chave);
        em_ordem(raiz->fd);
    }
}

//visita a arvore em pos ordem
void pos_ordem(nodo* raiz) {

    if (nodo != NULL) {
        pos_ordem(raiz->fe);
        pos_ordem(raiz->fd);
        printf("%d ", raiz->chave);
    }
}

//funcao que cria um nodo 

nodo* criar_nodo(int chave, nodo* fe, nodo* fd) {

    nodo* novo_nodo = (nodo*)malloc(sizeof(nodo));
    if (novo_nodo == NULL) //memoria insuficiente
        exit(1);
    
    novo_nodo->chave = chave;
    novo_nodo->fe = fe;
    novo_nodo->fd = fd;
    return novo_nodo;
}

nodo* inserir(nodo* raiz, int valor) {

    if (raiz == NULL)
        return criar_nodo(chave);

    if (chave < 0) {
        raiz->fe = inserir(raiz->fe, chave);
    } else if (chave > 0) {
        raiz->fd = inserir(raiz->fd, chave);
    } else 
        return 0;

    return raiz;
}

void liberar_arvore(nodo* raiz) {
    
    if (raiz != NULL) {
        liberar_arvore(raiz->fe);
        liberar_arvore(raiz->fd);
        free(raiz);
    }
}
