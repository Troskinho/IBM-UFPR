#include <stdio.h>
#include <stdlib.h>
#include "auxiliares.h"
#include "arvoreB.h"

//implementacao de funcoes auxiliares que foram necessarias 
void verifica_alocacao(void *nodo_teste) {
    if (nodo_teste == NULL) {
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
}

struct nodo* criar_nodo(int32_t t_arvore, bool eh_folha) {

    struct nodo* novo_nodo = (struct nodo*)malloc(sizeof(struct nodo));
    verifica_alocacao(novo_nodo);

    novo_nodo->n = 0;
    novo_nodo->eh_folha = eh_folha;
    
    //segue a descricao da arvore b de que o numero maximo de chaves é 2t-1
    novo_nodo->chave = (int32_t*)malloc((2 * t_arvore - 1) * sizeof(int32_t));
    verifica_alocacao(novo_nodo->chave);

    //segue a descricao da arvore b de que o numero maximo de ponteiros para filhos é 2t
    novo_nodo->filhos = (struct nodo**) malloc((2 * t_arvore) * sizeof(struct nodo*));
    verifica_alocacao(novo_nodo->filhos);

    //iniciar os filhos do novo_nodo como NULL para evitar segment fault
    for (int32_t i = 0; i < (2 * t_arvore); i++) {
        novo_nodo->filhos[i] = NULL;
    }

    return novo_nodo;
}

void dividir_filho(struct nodo* nodo_pai, int32_t i, struct nodo* nodo_filho, int32_t t) {
    
    //esse novo_ filho eh um nodo criado para poder armazenar as t-1 chaves maiores que o nodo_filho
    struct nodo* novo_filho = criar_nodo(t, nodo_filho->eh_folha);
    novo_filho->n = t -1;

    //chaves vao para novo filho
    for (int32_t j = 0; j < t -1; j++) {
        novo_filho->chave[j] = nodo_filho->chave[j + t];
    }

    //caso novo_filho nao seja folha, recebe os filos do nodo_dilho anterior
    if (!nodo_filho->eh_folha) {
        for (int32_t j = 0; j < t; j++) {
            novo_filho->filhos[j] = nodo_filho->filhos[j + t];
        }
    }

    nodo_filho->n = t -1;

    for (int32_t j = nodo_pai->n; j>= i + 1; j--) {
        nodo_pai->filhos[j + 1] = nodo_pai->filhos[j];
    }

    nodo_pai->filhos[i + 1] = novo_filho;

    //a chave mediana do nodo_filho sobe para nodo_pai
    for (int32_t j = nodo_pai->n - 1; j >= i; j--) {
        nodo_pai->chave[j + 1] = nodo_pai->chave[j];
    }

    nodo_pai->chave[i] = nodo_filho->chave[t - 1];
    nodo_pai->n = nodo_pai-> n + 1;
}

void inserir_nao_cheio(struct nodo* nodo, int32_t chave, int32_t t) {

    int32_t i = nodo->n - 1;

    if (nodo->eh_folha) {
        //deslocando chaves maiores para liberar espaco
        while (i >= 0 && chave < nodo->chave[i]) {
            nodo->chave[i + 1] = nodo->chave[i];
            i--;
        }
        nodo->chave[i + 1] = chave;
        nodo->n = nodo->n + 1;
    } else {
        //encontra o filho certo para a chave
        while (i >= 0 && chave < nodo->chave[i]) {
            i--;
        }
        i++;
        //verifica se o filho esta cheio, se sim o divide
        if (nodo->filhos[i]->n == 2 * t -1) {
            dividir_filho(nodo, i, nodo->filhos[i], t);

            if (chave > nodo->chave[i]) {
                i++;
            }
        }    
        inserir_nao_cheio(nodo->filhos[i], chave, t);
    }
}

void imprimir_nodo_ordem(struct nodo* nodo) {

    if (nodo == NULL) return;

    //ideia de imprimir de maneira recursiva
    for (int32_t i = 0; i < nodo->n; i++) {
        if (!nodo->eh_folha) {
            imprimir_nodo_ordem(nodo->filhos[i]);
        }
        printf("%d ", nodo->chave[i]);
    }

    if (!nodo->eh_folha) {
        imprimir_nodo_ordem(nodo->filhos[nodo->n]);
    }
}

void deletar_nodo(struct nodo* nodo) {

    if (nodo == NULL) return;

    if (!nodo->eh_folha) {
            for (int32_t i = 0; i <= nodo->n; i++) {
                deletar_nodo(nodo->filhos[i]);
            }
    }
    free(nodo->chave);
    free(nodo->filhos);
    free(nodo);
}