#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//estrutura do nodo
typedef struct nodo {
    int chave;
    struct nodo* fe;
    struct nodo* fd;
} nodo;

//funcao de travessia que percorre em "Pre ordem"
void pre_ordem(nodo* no) {
    if (no != NULL) {
        printf("%d ", no->chave); //imprime o valor da raiz e dps de forma recursiva visita os valores dos filhos esquerdo > direito
        pre_ordem(no->fe);
        pre_ordem(no->fd);
    }
}

//funcao de busca de um valor em profundidade
//usa a ideia de pre ordem apesar de nao usar a funcao de pre ordem
//funciona de maneira recursiva
nodo* depth_first(nodo* no, int valor) {
    if (no == NULL)  // nodo nao existe retorna nulo
        return NULL;

    if (no->chave == valor)  //valor armazenado no nodo igual ao valor buscado retorna o nodo
        return no;

    nodo* retornar;  //cria um nodo para fazer a verificacao nos filhos do nodo inicial
    retornar = depth_first(no->fe, valor); //aplica dfs no filho esquerdo
    if (retornar != NULL)                // se nao for nulo, retorna o nodo criado
        return retornar;
    retornar = depth_first(no->fd, valor); //aplica dfs no filho direito

    return retornar;
}

//criacao de pilha para fazer a dfs iterativa e nao recursiva
struct nodo_pilha {
    struct nodo* arvore_nodo; //ponteiro para o no da arvore
    struct nodo_pilha* prox; //ponteiro para o prox item da pilha
};

//verificacao em caso de pilha vazia
int vazio(struct nodo_pilha* topo) {
    return (topo == NULL);
}

//funcao para adicionar um elemento no topo da pilha
//nome comum eh push por isso 
void push(struct nodo_pilha** topo, nodo* arvore_nodo) {
    struct nodo_pilha* novo = (struct nodo_pilha*)malloc(sizeof(struct nodo_pilha)); //aloca memoria para nvoo nodo na pilha
    if (novo == NULL) //verifica se esse novo elemento existe
        exit(1);
    
    novo->arvore_nodo = arvore_nodo; //novo nodo recebe os valores apontados e o novo topo é esse adicionado
    novo->prox = *topo;
    *topo = novo;
}

//funcao apra remover e retornar o elemento no topo da pilha
//pop
nodo* pop(struct nodo_pilha** topo) {
    if (vazio(*topo))
        return NULL;        //se o topo estiver vazio ent nao tem elemnto na pilha e nada para remover
    struct nodo_pilha* aux = *topo;
    *topo = (*topo)->prox;      //troca
    nodo* removido = aux->arvore_nodo;
    free(aux);  //libera memoria do no da pilha 
    return removido;

}

void liberar_pilha(struct nodo_pilha** topo) {
    while(!vazio(*topo)) {
        pop(topo);      //vai chamando a funcao pop para liberar a memoria da pilha toda
    }
}

//dfs de forma iterativa usando a pilha
nodo* dfs_iterativo(nodo* raiz, int valor) {
    if (raiz == NULL)
        return NULL;        //caso base

    struct nodo_pilha* pilha = NULL;    //inicializa a pilha
    push(&pilha, raiz);     //insere a raiz na pilha

    while (!vazio(pilha)) {         //executa enquanto a pilha nao estiver vazia
        nodo* atual = pop(&pilha); //pega o nodo do topo
        if (atual->chave == valor) {        //verific se o valor foi encontrado
            liberar_pilha(&pilha);          //esvazia a pilha restante para evitar vazamento de memoria
            return atual;
        }

        //precisamos empilhar primeiro o filho direito para que o esquerdo fique no topo
        if (atual->fd != NULL) 
            push(&pilha, atual->fd);
        if (atual->fe != NULL)
            push(&pilha, atual->fe);
    }

    return NULL; //caso o valor nao seja encontrado de forma alguma, retorna null
}
