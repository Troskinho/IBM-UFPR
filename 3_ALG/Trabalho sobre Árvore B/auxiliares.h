#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#include "arvoreB.h"

struct item_fila {
    struct nodo* n;
    int32_t nivel;
};


//funcao para realizar as verificacoes de alocacao
void verifica_alocacao(void *nodo_teste);

//funcao que aloca memoria para um novo nodo
struct nodo* criar_nodo(int32_t t_arvore, bool eh_folha);

//dividimos nodo_filho, que eh filho do nodo_pai pois ele esta cheio
void dividir_filho(struct nodo* nodo_pai, int32_t i, struct nodo* nodo_filho, int32_t t);

//insere a chave em um nodo nao cheio jah verificado
void inserir_nao_cheio(struct nodo* nodo, int32_t chave, int32_t t);

//fucnao para imprimir nodo em ordem
void imprimir_nodo_ordem(struct nodo* nodo);

//funcao para liberar a memoria dos nodos antes de liberar a memoria da arvore
//nao tenho certeza se essa implementacao eh mais recomendada mas pensei na modularizacao do codigo
void deletar_nodo(struct nodo* nodo);



#endif