#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum { VERMELHO, PRETO  } Cor;

typedef struct nodo_rb {
    int chave;
    Cor cor;
    struct nodo_rb* fe;
    struct nodo_rb* fd;
} nodo_rb;

typedef struct nodo_dtq {
    int n;
    int chaves[3];
    struct nodo_dtq* filhos[4];
    bool eh_folha;
} nodo_dtq;

//converte da arvore red black para a arvore 2-3-4
nodo_dtq* converte_dtq(nodo_rb* raiz_rb) {
    //verificacao primaria
    if (raiz_rb == NULL)
        return NULL;
    
    //aloca um novo no da arvore 234
    nodo_dtq* novo = (nodo_dtq*)malloc(sizeof(nodo_dtq));
    //inicializa todos os ponteiros de filhos como NULL, evitando lixo de
    //memoria nos casos que nao usam os 4 slots (n == 1 ou n == 2)
    novo->filhos[0] = NULL;
    novo->filhos[1] = NULL;
    novo->filhos[2] = NULL;
    novo->filhos[3] = NULL;

    //caso 1 = nodo preto com dois filhos vermelhos
    if (raiz_rb->fe != NULL && raiz_rb->fe->cor == VERMELHO &&
        raiz_rb->fd != NULL && raiz_rb->fd->cor == VERMELHO) {
            
            novo->n =3;
            novo->chaves[0] = raiz_rb->fe->chave;
            novo->chaves[1] = raiz_rb->chave;
            novo->chaves[2] = raiz_rb->fd->chave;

            //filhos dos nodos vermelhos viram os 4 filhos do novo nodo
            novo->filhos[0] = converte_dtq(raiz_rb->fe->fe);
            novo->filhos[1] = converte_dtq(raiz_rb->fe->fd);
            novo->filhos[2] = converte_dtq(raiz_rb->fd->fe);
            novo->filhos[3] = converte_dtq(raiz_rb->fd->fd);
    }
    //caso 2 = nodo preto tem um filho vermelho a esquerd
    else if (raiz_rb->fe != NULL && raiz_rb->fe->cor == VERMELHO) {
        
        novo->n = 2;
        novo->chaves[0] = raiz_rb->fe->chave;
        novo->chaves[1] = raiz_rb->chave;

        novo->filhos[0] = converte_dtq(raiz_rb->fe->fe);
        novo->filhos[1] = converte_dtq(raiz_rb->fe->fd);
        novo->filhos[2] = converte_dtq(raiz_rb->fd);    //filho direito do nodo preto
    }
    //caso 3 = nodo preto tem um filho direito a direita
    else if (raiz_rb->fd != NULL && raiz_rb->fd->cor == VERMELHO) {

        novo->n = 2;
        novo->chaves[0] = raiz_rb->chave;
        novo->chaves[1] = raiz_rb->fd->chave;

        novo->filhos[0] = converte_dtq(raiz_rb->fe);
        novo->filhos[1] = converte_dtq(raiz_rb->fd->fe);
        novo->filhos[2] = converte_dtq(raiz_rb->fd->fd);
    }
    //caso 4 = nodo preto nao tem filhos vermelhos
    else {
        novo->n = 1;
        novo->chaves[0] = raiz_rb->chave;

        novo->filhos[0] = converte_dtq(raiz_rb->fe);
        novo->filhos[1] = converte_dtq(raiz_rb->fd);
    }

    novo->eh_folha = (novo->filhos[0] == NULL) ? true : false;
    return novo;
}

//funcao pra criar um nodo que seja da redblack
nodo_rb* criar_nodo_rb(int chave, Cor cor) {
    nodo_rb* nodo = (nodo_rb*)malloc(sizeof(nodo_rb));
    nodo->chave = chave;
    nodo->cor = cor;
    nodo->fe = NULL;
    nodo->fd = NULL;
    return nodo;
}

nodo_rb* converter_rb(nodo_dtq* no_dtq) {
    if (no_dtq == NULL) 
        return NULL;

    nodo_rb* raiz_rb = NULL;

    //caso 1 = 3 chaves > um preto com dois filhos vermelhos
    if (no_dtq->n == 3) {
        raiz_rb = criar_nodo_rb(no_dtq->chaves[1], PRETO); //chave do meio eh preta
        
        raiz_rb->fe = criar_nodo_rb(no_dtq->chaves[0], VERMELHO);
        raiz_rb->fd = criar_nodo_rb(no_dtq->chaves[2], VERMELHO);

        //reconecta os 4 filhos originais
        raiz_rb->fe->fe = converter_rb(no_dtq->filhos[0]);
        raiz_rb->fe->fd = converter_rb(no_dtq->filhos[1]);
        raiz_rb->fd->fe = converter_rb(no_dtq->filhos[2]);
        raiz_rb->fd->fd = converter_rb(no_dtq->filhos[3]);
    }
    //caso 2 = 2 chaves > um preto com filho vermelho a esquerda
    else if (no_dtq->n == 2) {
        raiz_rb = criar_nodo_rb(no_dtq->chaves[1], PRETO);

        raiz_rb->fe = criar_nodo_rb(no_dtq->chaves[0], VERMELHO);

        //reconecta os 3 filhos originais
        raiz_rb->fe->fe = converter_rb(no_dtq->filhos[0]);
        raiz_rb->fe->fd = converter_rb(no_dtq->filhos[1]);
        raiz_rb->fd = converter_rb(no_dtq->filhos[2]);
    }
    //caso 3 = 1 chave > um unico nodo preto
    else if (no_dtq->n == 1) {
        raiz_rb = criar_nodo_rb(no_dtq->chaves[0], PRETO);

        //reconecta os 2 filhos originais
        raiz_rb->fe = converter_rb(no_dtq->filhos[0]);
        raiz_rb->fd = converter_rb(no_dtq->filhos[1]);
    }
    return raiz_rb;
}

void liberar_rb(nodo_rb* x) {
    if (x == NULL) return;
    liberar_rb(x->fe);
    liberar_rb(x->fd);
    free(x);
}
 
void liberar_dtq(nodo_dtq* x) {
    if (x == NULL) return;
    for (int i = 0; i < 4; i++) {
        liberar_dtq(x->filhos[i]);
    }
    free(x);
}
