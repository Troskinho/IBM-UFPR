#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
    int chave;
    struct nodo* fe;
    struct nodo* fd;
    struct nodo* pai;   //ponteiro para o nodo pai 
} nodo;

typedef struct arvore {
    nodo* raiz;
} arvore;

void inserir(arvore* T, int chave) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    if (novo == NULL)   
        return;     //novo nodo nao existe ent nao aloca nada
    novo->chave = chave;
    novo->fe = NULL;
    novo->fd = NULL; //ponteiro pai eh definido com base no valor da chave do nodo novo
                    //para poder inserir no local certo da arvore

    nodo* atual = T->raiz;
    nodo* pai = NULL;

    //percorre a arvore ate encontrar um espaco vazio
    while (atual != NULL) {
        pai = atual;        //guarda referencia antes de comecar a descer a arvore

        if (novo->chave < atual->chave) {
            atual = atual->fe;  //desce para subarvore a esquerda
        } else {
            atual = atual->fd;  //desce para subarvore a direita
        }
    }
    //confirma quem eh o pai do novo nodo
    novo->pai = pai;

    //parte da insercao de fato
    if (pai == NULL) {
        T->raiz = novo;     //se o pai for nulo entao  arvore estava vaazia
                            //novo nodo eh a raiz
    } else {
        if (novo->chave < pai->chave) {
            pai->fe = novo;     //conecta ao filho esquerdo
        } else {
            pai->fd = novo;     //conecta ao filho direito
        }
    }
}

nodo* buscar(nodo* raiz, int chave) {
    //caso base, raiz nula ou chave encontrada
    if (raiz == NULL || raiz->chave == chave) {
        return raiz;
    }

    //decidir para qual caminho seguir, direita ou esquerda
    if (chave < raiz->chave)
        return buscar(raiz->fe, chave);
    return buscar(raiz->fd, chave);
}

nodo* maximo(nodo* no) {
    if (no == NULL) 
        return NULL;        //subarvore vazia e nao ha maximo
    
    while (no->fd != NULL)
        no = no->fd;

    return no;
}

nodo* minimo(nodo* no) {
    if (no == NULL)
        return NULL;    //subarvore vazia e nao ha minimo

    while (no->fe != NULL)
        no = no->fe;

    return no;
}
