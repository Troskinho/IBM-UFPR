#include <stdlib.h>
#include <stdio.h>
#include "arvoreB.h"
#include "auxiliares.h"

static void teste_busca(struct arvoreB* arvore, int32_t chave) {

    int32_t idx = -1;
    struct nodo* resultado = buscarArvoreB(arvore, chave, &idx);
    if (resultado != NULL) {
        printf("Busca(%d): encontrado no indice %d do nodo\n", chave, idx);
    } else {
        printf("Busca(%d): nao encontrado\n", chave);
    }
}

static void teste_exemplo(void) {

    struct arvoreB* arvore = criarArvoreB(2);

    int32_t chaves[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int32_t n = (int32_t)(sizeof(chaves) / sizeof(chaves[0]));

    for (int32_t i = 0; i < n; i++) {
        inserirArvoreB(arvore, chaves[i]);
    }

    imprimirArvoreB(arvore);

    printf("Em ordem: ");
    imprimirEmOrdem(arvore);

    teste_busca(arvore, 7);
    teste_busca(arvore, 1);
    teste_busca(arvore, 12);
    teste_busca(arvore, 99);

    deletarArvore(arvore);
}

static void teste_fora_ordem(void) {

    struct arvoreB* arvore = criarArvoreB(3);

    int32_t chaves[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int32_t n = (int32_t)(sizeof(chaves) / sizeof(chaves[0]));
    
    for (int32_t i = 0; i < n; i++) {
        inserirArvoreB(arvore, chaves[i]);
    }

    imprimirArvoreB(arvore);

    printf("EM ordem: ");
    imprimirEmOrdem(arvore);

    teste_busca(arvore, 6);
    teste_busca(arvore, 17);
    teste_busca(arvore, 99);

    deletarArvore(arvore);
}

static void teste_decrescente(void) {

    struct arvoreB* arvore = criarArvoreB(2);

    for (int32_t i = 15; i >= 1; i--) {
        inserirArvoreB(arvore, i);
    }
    
    imprimirArvoreB(arvore);

    printf("Em ordem: ");
    imprimirEmOrdem(arvore);

    teste_busca(arvore, 1);
    teste_busca(arvore, 15);
    teste_busca(arvore, 8);

    deletarArvore(arvore);

}

static void teste_largura(void) {

    struct arvoreB* arvore = criarArvoreB(5);

    for (int32_t i = 1; i <= 30; i++) {
        inserirArvoreB(arvore, i);
    }

    imprimirArvoreB(arvore);

    printf("Em ordem: ");
    imprimirEmOrdem(arvore);

    teste_busca(arvore, 15);
    teste_busca(arvore, 30);
    teste_busca(arvore, 0);

    deletarArvore(arvore);
}

static void teste_elemento_unico(void) {

    struct arvoreB* arvore = criarArvoreB(2);

    inserirArvoreB(arvore, 42);

    imprimirArvoreB(arvore);

    printf("Em ordem: ");
    imprimirEmOrdem(arvore);

    teste_busca(arvore, 42);
    teste_busca(arvore, 1);
    
    deletarArvore(arvore);
}

int main(void) {
    teste_exemplo();
    teste_fora_ordem();
    teste_decrescente();
    teste_largura();
    teste_elemento_unico();

    return 0;
}