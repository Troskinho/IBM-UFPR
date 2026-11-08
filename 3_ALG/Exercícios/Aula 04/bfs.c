#include <stdlib.h>
#include <stdio.h>

//estrutura dos nos da arvore
typedef struct nodo{ 
    int chave;
    struct nodo* fe;
    struct nodo* fd;
} nodo;

//estrutura dos nos das filas
struct nodo_fila {
    struct nodo* arvore_nodo;
    struct nodo_fila* prox;
};

//estrutura de controle de elementos da fila
struct fila {
    struct nodo_fila* cabeca;
    struct nodo_fila* cauda;
};

//funcao para criar a fila vazia
struct fila* criar_fila(void) {
    struct fila* f = (struct fila*)malloc(sizeof(struct fila));
    if ( f == NULL) exit(1);   //se tenta adicionar um elemento que n existe, sai da funcao
    f->cabeca = NULL;
    f->cauda = NULL;
    return f;
}

//verifica se o estado atual da fila é vazio
int fila_vazia(struct fila* f) {
    return (f->cabeca == NULL);
}

//insere um novo elemento no final da fila
void enfileirar(struct fila* f, nodo* arvore_nodo) {
    struct nodo_fila* novo = (struct nodo_fila*)malloc(sizeof(struct nodo_fila));
    novo->arvore_nodo = arvore_nodo;
    novo->prox = NULL; //armazena memoria para o novo nodo

    //se a fila atual for vazia, o novo no e a cabeça e a cauda ao mesm otempo
    if (f->cauda == NULL) {
        f->cabeca = novo;
        f->cauda = novo;
    } else {
        //caso contrario, adiciona no final e atualiza a cauda
        f->cauda->prox = novo;
        f->cauda = novo;
    }
}

//funcao para remover a cabeca da fila
//retorna esse valor que era a cabeca
nodo* remover_cabeca(struct fila* f) {
    if (fila_vazia(f)) {
        return NULL;
    }

    struct nodo_fila* aux = f->cabeca;
    nodo* removido = aux->arvore_nodo;

    f->cabeca = f->cabeca->prox;

    if (f->cabeca == NULL) 
        f->cauda = NULL; //cauda sera nula caso cabeca seja nula

    free(aux); //libera o no da fila da memoria
    return removido; 
}

//funcao para percorrer a arvore em largura
//imprime o elemento do nodo para garantir que achou eçe
void percurso_largura(nodo* raiz) {
    if (raiz == NULL)  //verificacao inicial da raiz
        return;

    //inicializacao da fila
    struct fila* f = criar_fila();

    enfileirar(f, raiz); // enfileirar

    while (!fila_vazia(f)) {         //enquanto a fila nao for vazia
        nodo* n = remover_cabeca(f);
        printf("%d, ", n->chave);

        if (n->fe != NULL)          //se n possuir filho esquerdo
            enfileirar(f, n->fe);
        
        if (n->fd != NULL)          //se n possuir filho direito
            enfileirar(f, n->fd);
    }
    free(f);
}

//liberar a memoria alocada para a fila
void liberar_fila(struct fila* f) {
    while (!fila_vazia(f)) {
        remover_cabeca(f);
    }
    free(f);
}

//funcao que faz a busca usando o recurso de busca em largura/
//usa umka fila para ser implementada
nodo* bfs(nodo* raiz, int valor) {
    //se a arvore estiver vazia retorna null pois nao foi encontrado
    if (raiz == NULL) 
        return NULL;

    //inicializa a fila
    struct fila* f = criar_fila();
    enfileirar(f, raiz);

    while (!fila_vazia(f)) {
        nodo* n = remover_cabeca(f);

        if (n->chave == valor) {
            liberar_fila(f);
            return n;
        }
        if (n->fe != NULL) {
            enfileirar(f, n->fe);
        }
        if (n->fd != NULL) {
            enfileirar(f, n->fd);
        }
    }
    //se ele sair do while a fila esvaziou e não encontrou o valor
    //libera a memopria de controle da fila
    free(f);

    return NULL;
}
