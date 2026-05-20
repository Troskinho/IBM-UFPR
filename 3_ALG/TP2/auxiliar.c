#include <stdio.h>
#include <stdlib.h>
#include "auxiliar.h"
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

void imprimir_nodo_ordem(struct nodo* nodo, bool* primeiro) {

    if (nodo == NULL) return;

    //ideia de imprimir de maneira recursiva
    for (int32_t i = 0; i < nodo->n; i++) {
        if (!nodo->eh_folha) {
            imprimir_nodo_ordem(nodo->filhos[i], primeiro);
        }
        if (!*primeiro) printf("  ");
        printf("%d", nodo->chave[i]);
        *primeiro = false;    
    }

    if (!nodo->eh_folha) {
        imprimir_nodo_ordem(nodo->filhos[nodo->n], primeiro);
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

//retorna a maior chave da subarvore, o predecessor do nodo atual
int32_t predecessor(struct nodo* nodo) {
    while (!nodo->eh_folha)
        nodo = nodo->filhos[nodo->n];
    return nodo->chave[nodo->n - 1];
}

//retirna a menor chave da subarvore, o sucessor do nodo atual
int32_t sucessor(struct nodo* nodo) {
    while (!nodo->eh_folha)
        nodo = nodo->filhos[0];
    return nodo->chave[0];
}

void mesclar_filhos(struct nodo* nodo_pai, int32_t i, int32_t t) {
    
    struct nodo* esquerdo = nodo_pai->filhos[i];
    struct nodo* direito = nodo_pai->filhos[i + 1];

    //chave separadora do pai no filho esdquerdo
    esquerdo->chave[t -1] = nodo_pai->chave[i];

    //copia a chave do filho direito para o esquerdo
    for (int32_t j = 0; j < direito->n; j++) {
        esquerdo->chave[j + t] = direito->chave[j];
    }

    //copia os filgos do direito para o esquerdo caso não seja folha
    if (!esquerdo->eh_folha) {
        for (int32_t j = 0; j <= direito->n; j++)
            esquerdo->filhos[t + j] = direito->filhos[j];
    }

    esquerdo->n = 2 * t - 1;

    for (int32_t j = i; j < nodo_pai->n - 1; j++) {
        nodo_pai->chave[j] = nodo_pai->chave[j + 1];
    }
    //remove a chave separadora do pai
    for (int32_t j = i + 1; j < nodo_pai->n; j++) {
        nodo_pai->filhos[j] = nodo_pai->filhos[j + 1];
    }

    nodo_pai->filhos[nodo_pai->n] = NULL;
    nodo_pai->n--;

    free(direito->chave);
    free(direito->filhos);
    free(direito);
}

void remover_nodo(struct nodo* nodo, int32_t chave, int32_t t) {

    int32_t i = 0;

    //encontra a posicao da chave ou do nodo filho para descer
    while(i < nodo->n && chave > nodo->chave[i])
        i++;
    
    //caso da chave encontrada no nodo atual
    if (i < nodo->n && nodo->chave[i] == chave) {

        if (nodo->eh_folha) {
            //eh folha? sim = apenas remove
            for (int32_t j = i; j < nodo->n - 1; j++)
                nodo->chave[j] = nodo->chave[j + 1];
            nodo->n--;
        } else {
            struct nodo* filho_esquerdo = nodo->filhos[i];
            struct nodo* filho_direito = nodo->filhos[i + 1];

            if (filho_esquerdo->n >= t) {
                //caso do predecessor
                int32_t pred = predecessor(filho_esquerdo);
                nodo->chave[i] = pred;
                remover_nodo(filho_esquerdo, pred, t);
            } else if (filho_direito->n >= t) {
                //caso do sucessor
                int32_t suces = sucessor(filho_direito);
                nodo->chave[i] = suces;
                remover_nodo(filho_direito, suces, t);
            } else {
                //caso de mesclar os filhos
                mesclar_filhos(nodo, i , t);
                remover_nodo(nodo->filhos[i], chave, t);
            }
        } 
    } else {
        //chave n esta no nodo atual e precisa descer na arvore

        if (nodo->eh_folha)
            return;
        
        struct nodo* filho = nodo->filhos[i];

        if (filho->n < t) {

            if (i > 0 && nodo->filhos[i - 1]->n >= t) {
                //rotacao a partir do filho esquerdo
                struct nodo* irmao = nodo->filhos[i - 1];

                //desloca chaves do filho para a direita
                for (int32_t j = filho->n; j > 0; j--)
                    filho->chave[j] = filho->chave[j - 1];
                //desloca filhos do filho para a direita
                if (!filho->eh_folha)
                    for(int32_t j = filho->n + 1; j > 0; j--)
                        filho->filhos[j] = filho->filhos[j - 1];
                
                //desce a chave do pai para o filho
                filho->chave[0] = nodo->chave[i - 1];
                //sobe a ultima chave do irmao para o pai
                nodo->chave[i - 1] = irmao->chave[irmao->n - 1];

                //move o ultimo dilgo do irmao para o filho
                if (!irmao->eh_folha) {
                    filho->filhos[0] = irmao->filhos[irmao->n];
                    irmao->filhos[irmao->n] = NULL;
                }

                filho->n++;
                irmao->n--;
            } else if (i < nodo->n && nodo->filhos[i + 1]->n >= t) {
                //rotacao a partir do filho direito
                struct nodo* irmao = nodo->filhos[i + 1];

                //desce a chave do pai para o filho
                filho->chave[filho->n] = nodo->chave[i];
                //sobe a primeira chave do irmao para o pai
                nodo->chave[i] = irmao->chave[0];

                //move o primeiro filho do irmao para o filho
                if (!irmao->eh_folha) {
                    filho->filhos[filho->n + 1] = irmao->filhos[0];
                    //desloca filhos do itmao para a esquerda
                    for (int32_t j = 0; j < irmao->n; j++)
                        irmao->filhos[j] = irmao->filhos[j + 1];
                    irmao->filhos[irmao->n] = NULL;
                }

                //desloca as chves do irmao para a esquerda
                for (int32_t j = 0; j < irmao->n - 1; j++)
                    irmao->chave[j] = irmao->chave[j + 1];

                filho->n++;
                irmao->n--; 
            } else {
                //merge usando o irmao esquerdo se existir
                if (i > 0) {
                    mesclar_filhos(nodo, i - 1, t);
                    i = i - 1;
                } else {
                    mesclar_filhos(nodo, i, t);
                    //filho jah eh nodo->filhos[i] apos o merge
                }
            }
        } 
        if (i > nodo->n) i = nodo->n;
        remover_nodo(nodo->filhos[i], chave, t);
    }
}