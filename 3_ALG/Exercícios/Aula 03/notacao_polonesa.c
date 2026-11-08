#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo {
    char dado[30];
    struct nodo* fe;
    struct nodo* fd;
} nodo;

nodo* criar_nodo(char* dado, nodo* fe, nodo* fd) {

    nodo* novo = (nodo*)malloc(sizeof(nodo));
    if (novo != NULL) {
        strcpy(novo->dado, dado);
        novo->fe = fe;
        novo->fd = fd;
    }
    return novo;
}

int operador(char* codigo) {

    return (strlen(codigo) == 1 && (codigo[0] == '+' || codigo[0] == '-' || codigo[0] == '*' || codigo[0] == '/'));

}

nodo* criar_arvore(char* valor, int* valido) {

    if (valor == NULL) {
        *valido = 0;
        return NULL;
    }

    nodo* raiz = criar_nodo(valor, NULL, NULL);

    if (operador(valor)) {
        raiz->fe = criar_arvore(strtok(NULL, " \n"), valido);
        raiz->fd = criar_arvore(strtok(NULL, " \n"), valido);
    }

    return raiz;
}

void pre_ordem(nodo* raiz) {

    if (raiz != NULL) {
        printf("%s ", raiz->chave);
        pre_ordem(raiz->fe);
        pre_ordem(raiz->fd);
    }
}

void em_ordem(nodo* raiz) {

    if (raiz != NULL) {
        em_ordem(raiz->fe);
        printf("%s ", raiz->chave);
        em_ordem(raiz->fd);
    }
}

void pos_ordem(nodo* raiz) {

    if (raiz != NULL) {
        pos_ordem(raiz->fe);
        pos_ordem(raiz->fd);
        printf("%s ", raiz->chave);
    }
}

void liberar_arvore(nodo* raiz) {
    
    if (raiz != NULL) {
        liberar_arvore(raiz->fe);
        liberar_arvore(raiz->fd);
        free(raiz);
    }
}

double verifica_resultado(nodo* raiz) {

    if (raiz == NULL) return 0.0;

    if (!operador(raiz->dado)) {
        return atof(raiz->dado);
    }

    double valor_esq = verifica_resultado(raiz->fe);
    double valor_dir = verifica_resultado(raiz->fd);

    switch(raiz->dado[0]) {
        case '+': return valor_esq + valor_dir;
        case '-': return valor_esq - valor_dir;
        case '*': return valor_esq * valor_dir;
        case '/': return valor_esq / valor_dir;
    }

    return 0.0;
}
