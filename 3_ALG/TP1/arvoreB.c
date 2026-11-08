#include <stdio.h>
#include <stdlib.h>
#include "arvoreB.h"
#include "auxiliares.h"

//implementadas as funcoes descritas no enunciado do trabalho 

struct arvoreB* criarArvoreB(int32_t t_arvore) {

    struct arvoreB* nova_arvore = (struct arvoreB*)malloc(sizeof(struct arvoreB));
    verifica_alocacao(nova_arvore);

    nova_arvore->t_arvore = t_arvore;
    //usa a funcao criar_nodo e passa o booleano eh_folha como true, ja que eh a raiz
    nova_arvore->raiz = criar_nodo(t_arvore, true);

    return nova_arvore;
}

void inserirArvoreB(struct arvoreB* arvore, int32_t chave) {

    if (arvore == NULL || arvore->raiz == NULL) return;

    struct nodo* root = arvore->raiz;
    int32_t t = arvore->t_arvore;

    //if verifica se raiz esta cheia
    if (root->n == 2 * t - 1) {
        //se esta cheia cria uma nova raiz e divide a antiga
        struct nodo* nova_raiz = criar_nodo(t, false);
        arvore->raiz = nova_raiz;
        nova_raiz->filhos[0] = root;

        dividir_filho(nova_raiz, 0, root, t);

        inserir_nao_cheio(nova_raiz, chave, t);
    } else {
        inserir_nao_cheio(root, chave, t);
    }
}

void imprimirArvoreB(struct arvoreB* arvore){

    if (arvore->raiz == NULL) return;

    int32_t max_fila = 100; //numero exemplo
    struct item_fila* fila = (struct item_fila*) malloc(max_fila * sizeof(struct item_fila));
    verifica_alocacao(fila);
    
    int32_t inicio = 0, fim = 0;
    int32_t nivel_atual = -1;

    fila[fim].n = arvore->raiz;
    fila[fim].nivel = 0;
    fim++;

    while (inicio < fim) {

        struct nodo* nodo_atual = fila[inicio].n;
        int32_t nivel_nodo = fila[inicio].nivel;
        inicio++;

        //so pra seguir a formatacao solicitada no trabalho
        if (nivel_nodo > nivel_atual) {
            if (nivel_atual != -1) printf("\n");
            printf("----//----\n");
            printf("Nivel %d\n", nivel_nodo);
            printf("----//----\n");
            nivel_atual = nivel_nodo;
        } else if (inicio > 1) {
            printf("  ");
        }

        printf("%c (n:%d) [", nodo_atual->eh_folha ? 'F' : 'I', nodo_atual->n);
        for (int32_t i = 0; i < nodo_atual->n; i++) {
            printf("%d", nodo_atual->chave[i]);
            if (i < nodo_atual->n - 1) printf(" ");
        }
        printf("]");

        if (!nodo_atual->eh_folha) {
            for (int32_t i = 0; i <= nodo_atual->n; i++) {
                if (nodo_atual->filhos[i] != NULL) {
                    fila[fim].n = nodo_atual->filhos[i];
                    fila[fim].nivel = nivel_nodo + 1;
                    fim++;
                }
            }
        }
    }
    printf("\n");
    free(fila);
}

void imprimirEmOrdem(struct arvoreB* arvore) {
    imprimir_nodo_ordem(arvore->raiz);
    printf("\n");
}

struct nodo* buscarArvoreB(struct arvoreB* arvore, int32_t chave, int32_t* idxEncontrado) {

    struct nodo* nodo_atual = arvore->raiz;

    while (nodo_atual != NULL) {
        int32_t i = 0;

        //avanca enquanto a chave idx buscada eh maior que a atual
        while (i < nodo_atual->n && chave > nodo_atual->chave[i]) {
            i++;
       }

       if (i < nodo_atual->n && chave == nodo_atual->chave[i]) {
            *idxEncontrado = i;
            return nodo_atual;
       }

       if (nodo_atual->eh_folha) {
            *idxEncontrado = -1;
            return NULL;
       }

       nodo_atual = nodo_atual->filhos[i];
    }

    *idxEncontrado = -1;
    return NULL;
}

void deletarArvore(struct arvoreB* arvore) {

    if (arvore == NULL) return;

    deletar_nodo(arvore->raiz);
    free(arvore);
}
