#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
    int chave;
    struct nodo* fe;
    struct nodo* fd;
    struct nodo* pai;
} nodo; 

typedef struct arvore {
    nodo* raiz;
} arvore;

void transplantar(arvore* T, nodo* u, nodo* v) {
    //atualiza o ponteiro do pai de u para apontar para v
    if (u->pai == NULL) {
        T->raiz = v;        //se u nao tem pai, ele era a raiz, e v se torna a nova raiz
    }
    else if (u == u->pai->fe) {
        u->pai->fe = v; //se u era filho esquerdo do pai, o pai adota v como seu filho esquerdo
    } 
    else {
        u->pai->fd = v; //se u era filho direito do pai, o pai adota v como seu filho direito
    }

    //atualiza o ponteiro do pai de v para apontar para o antigo pai de u
    if (v != NULL) {
        v->pai = u->pai;
    }
}

nodo* minimo(nodo* no) {
    if (no == NULL)
        return NULL;    //subarvore vazia e nao ha minimo

    while (no->fe != NULL)
        no = no->fe;

    return no;
}

void excluir(arvore* T, nodo* z) {
    //caso em z nao possui filho esquerdo 
    if (z->fe == NULL) {
        transplantar(T, z, z->fd);
    }
    else if (z->fd == NULL) {
        transplantar(T, z, z->fe);
    }
    else {
        nodo* y = minimo(z->fd);    //encontra o sucessor de z 

        //se o sucessor y nao for imediato de z ele transplanta pelo filho imediato
        if (y != z->fd) {
            transplantar(T, y, y->fd);
            y->fd = z->fd;
            y->fd->pai = y;
            //y pega o filho direito original de z para si
        }
        //substitui z por y na arvore principal
        transplantar(T, z, y);
        //y pega o filho esquerdo original de z
        y->fe = z->fe;
        y->fe->pai = y;
    }
    free(z); //liberar a memoria do nodo excluido 
}
