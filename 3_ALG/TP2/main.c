#include <stdlib.h>
#include <stdio.h>
#include "arvoreB.h"
#include "auxiliar.h"

static void teste_busca(struct arvoreB* arvore, int32_t chave) {

    int32_t idx = -1;
    struct nodo* resultado = buscarArvoreB(arvore, chave, &idx);
    if (resultado != NULL) {
        printf("Busca(%d): encontrado no indice %d do nodo\n", chave, idx);
    } else {
        printf("Busca(%d): nao encontrado\n", chave);
    }
}

static void teste_remocao(struct arvoreB* arvore, int32_t chave) {
 
    bool removido = removerChaveArvoreB(arvore, chave);
    if (removido) {
        printf("Remocao(%d): removido com sucesso\n", chave);
    } else {
        printf("Remocao(%d): chave nao encontrada\n", chave);
    }
}

static void teste_exemplo_com_remocao(void) {
  
    struct arvoreB* arvore = criarArvoreB(2);
 
    int32_t chaves[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int32_t n = (int32_t)(sizeof(chaves) / sizeof(chaves[0]));
 
    for (int32_t i = 0; i < n; i++) {
        inserirArvoreB(arvore, chaves[i]);
    }
 
    printf("Arvore inicial:\n");
    imprimirArvoreB(arvore);
    imprimirEmOrdem(arvore);
 
    // Remove chave de folha
    teste_remocao(arvore, 1);
    printf("Apos remover 1:\n");
    imprimirArvoreB(arvore);
    imprimirEmOrdem(arvore);
 
    // Remove chave interna (deve usar predecessor ou sucessor)
    teste_remocao(arvore, 6);
    printf("Apos remover 6 (chave interna):\n");
    imprimirArvoreB(arvore);
    imprimirEmOrdem(arvore);
 
    // Tenta remover chave inexistente
    teste_remocao(arvore, 99);
 
    // Busca para confirmar estado
    teste_busca(arvore, 1);
    teste_busca(arvore, 6);
    teste_busca(arvore, 12);
 
    deletarArvore(arvore);
}

static void teste_fora_ordem_com_remocao(void) {
  
    struct arvoreB* arvore = criarArvoreB(3);
 
    int32_t chaves[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int32_t n = (int32_t)(sizeof(chaves) / sizeof(chaves[0]));
 
    for (int32_t i = 0; i < n; i++) {
        inserirArvoreB(arvore, chaves[i]);
    }
 
    printf("Arvore inicial:\n");
    imprimirArvoreB(arvore);
    imprimirEmOrdem(arvore);
 
    teste_remocao(arvore, 5);
    teste_remocao(arvore, 10);
    printf("Apos remover 5 e 10:\n");
    imprimirArvoreB(arvore);
    imprimirEmOrdem(arvore);
 
    teste_busca(arvore, 5);
    teste_busca(arvore, 10);
    teste_busca(arvore, 8);
 
    deletarArvore(arvore);
}

static void teste_largura(void) {

    struct arvoreB* arvore = criarArvoreB(5);

    for (int32_t i = 1; i <= 30; i++) {
        inserirArvoreB(arvore, i);
    }

    imprimirArvoreB(arvore);

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

    imprimirEmOrdem(arvore);

    teste_busca(arvore, 42);
    teste_busca(arvore, 1);
    
    deletarArvore(arvore);
}

static void teste_remocao_total(void) {

    struct arvoreB* arvore = criarArvoreB(2);

    int32_t chaves[] = {1, 2, 3, 4, 5};
    int32_t n = (int32_t)(sizeof(chaves) / sizeof(chaves[0]));

    for (int32_t i = 0; i < n; i++) {
        inserirArvoreB(arvore, chaves[i]);
    }

    printf("Arvore inicial:\n");
    imprimirArvoreB(arvore);
    imprimirEmOrdem(arvore);

    for (int32_t i = 0; i < n; i++) {
        teste_remocao(arvore, chaves[i]);
        printf("Apos remover %d:\n", chaves[i]);
        imprimirArvoreB(arvore);
        imprimirEmOrdem(arvore);
    }
    deletarArvore(arvore);
}

int main (void) {

    teste_exemplo_com_remocao();
    teste_fora_ordem_com_remocao();
    teste_largura();
    teste_elemento_unico();
    teste_remocao_total();

    return 0;
}