#ifndef AUXILIAR_H_
#define AUXILIAR_H_

#include <stdint.h>
#include "arvoreB.h"

struct item_fila {
    struct nodo* n;
    int32_t nivel;
};

//funcao para realizar as verificacoes na alocacao
void verifica_alocacao(void *nodo_teste);

//funcao que aloca memoria para um novo nodo
struct nodo* criar_nodo(int32_t t_arvore, bool eh_folha);

//dividimos nodo_filho, que eh filho do nodo_pai pois ele esta cheio
void dividir_filho(struct nodo* nodo_pai, int32_t i, struct nodo* nodo_filho, int32_t t);

//insere a chave em um nodo nao cheio jah verificado
void inserir_nao_cheio(struct nodo* nodo, int32_t chave, int32_t t);

//funcao para imprimir nodo em ordem
void imprimir_nodo_ordem(struct nodo* nodo, bool* primeiro);

//funcao para liberaar memoria dos nodos antes de liberar a memoria da arvore
//nao tenho certeza se essa implementacao eh a mais recomendada mas pensei na modularizacao do codigo
void deletar_nodo(struct nodo* nodo);

//função que calcula o nodo predecessor ao nodo atual
int32_t predecessor(struct nodo* nodo);

//funcao que calcula o nodo sucessor ao nodo atual
int32_t sucessor(struct nodo* nodo);

//funcao para mesclar o nodo filho i com o filho i+1 do nodo pai, apos a mesclagem o nodo filho i+1 eh liberado
void mesclar_filhos(struct nodo* nodo_pai, int32_t i, int32_t t);

//funcao para remover a chave chve do nodo
void remover_nodo(struct nodo* nodo, int32_t chave, int32_t t);


#endif
