#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum { VERMELHO, PRETO  } Cor;

typedef struct nodo {
    int chave;
    Cor cor;
    struct nodo* fe;
    struct nodo* fd;
    struct nodo* pai;
} nodo;

typedef struct arvore {
    nodo* raiz;
    nodo* sentinela;   
} arvore;

nodo* criar_nodo(int chave, Cor cor) {
    nodo* no = (nodo*)malloc(sizeof(nodo));
    no->chave = chave;
    no->cor = cor;
    no->fe = NULL;
    no->fd = NULL;
    no->pai = NULL;
    return no;
}

arvore* criar_arvore(void) {
    arvore* T = (arvore*)malloc(sizeof(arvore));
 
    nodo* sentinela = (nodo*)malloc(sizeof(nodo));
    sentinela->cor = PRETO;
    sentinela->fe = sentinela;
    sentinela->fd = sentinela;
    sentinela->pai = sentinela;
 
    T->sentinela = sentinela;
    T->raiz = sentinela;
 
    return T;
}

void redblack_fixup(arvore* T, nodo* z) {
    //o loop continua enquanto houver violacao de cor
    while (z->pai->cor == VERMELHO) {
        //se o pai de z eh filho esquerdo doa vo
        if (z->pai == z->pai->pai->fe) {
            nodo* tio = z->pai->pai->fd;    //o tio eh filho direito do avo

            //caso 1 = tio de z eh vermelho
            if (tio != T->sentinela && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;    //pai fica preto
                tio->cor = PRETO;       //o tio fica preto  
                z->pai->pai->cor = VERMELHO;    //o avo fica vermelho
                z = z->pai->pai;            //se algo estiver errado com o avo, consertamos depois
            }
            else {
                //caso 2 = tio de z eh preto ou sentinela, filho direito
                if (z == z->pai->fd) {
                    z = z->pai;
                    rotacao_esquerda(T, z);     //forma uma linha reta de descid
                }
                //caso 3 = tio de z eh preto, filho esquerdo
                z->pai->cor = PRETO;    //pai vira preto
                z->pai->pai->cor = VERMELHO;    //avo fica vermelho
                rotacao_direita(T, z->pai->pai); //rotaciona oa vo para a direita
            }
        }
        //se o pai de ze eh filho direito do avo
        else {
            nodo* tio = z->pai->pai->fe;    //tio eh filho esquerdo do avo
            //caso 1 = tio de z eh vermelho
            if (tio != T->sentinela && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else {
                //caso 2 = tio de z eh preto, filho esquerdo
                if (z == z->pai->fe) {
                    z = z->pai;
                    rotacao_direita(T, z);
                }
                //caso 3 = tio de z eh preto, filho direito
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_esquerda(T, z->pai->pai);
            }
        }
    }
    //a raiz deve ser sempre preta
    T->raiz->cor = PRETO;
}

void rotacao_direita(arvore* T, nodo* y) {
    //x eh o filho esquerdo, que sobe e toma o lugar de y
    nodo* x = y->fe;
    //o filho direito de x passa a ser filho esquerdo de y
    y->fe = x->fd;
    //se a subarvore nao for vazia (sentinela), atualizamos o seu pai
    if (x->fd != T->sentinela) {
        x->fd->pai = y;
    }
    //x sobe e assume o lugar do antigo pai de y
    x->pai = y->pai;
    //reconectamos o restante da arvore com x
    if (y->pai == T->sentinela) {
        //se y era raiz, agora x eh raiz
        T->raiz = x;
    }
    else if (y == y->pai->fd) {
        //se y era filho da direita, x entra na direita
        y->pai->fd = x;
    }
    else {
        //se y era filho da esquerda, x entra na esquerda
        y->pai->fe = x;
    }
    //y desce e vira filho direito de x
    x->fd = y;
    y->pai = x;         //x como pai de y
}
 
void rotacao_esquerda(arvore* T, nodo* y) {
    nodo* x = y->fd;
    y->fd = x->fe;
 
    if (x->fe != T->sentinela) {
        x->fe->pai = y;
    }
    x->pai = y->pai;
 
    if (y->pai == T->sentinela) {
        T->raiz = x;
    }
    else if (y == y->pai->fe) {
        y->pai->fe = x;
    }
    else {
        y->pai->fd = x;
    }
    x->fe = y;
    y->pai = x;
}
 
void insercao_redblack(arvore* T, int c) {
    //cria o novo nodo, que nasce vermelho
    nodo* z = criar_nodo(c, VERMELHO);
    z->fe = T->sentinela;
    z->fd = T->sentinela;
 
    //variaveis para navegacao
    nodo* x = T->raiz;          //desce pela arvore
    nodo* y = T->sentinela;     //o pai de x que desce
 
    //busca o local como uma BST normal
    while (x != T->sentinela) {
        y = x;      //guarda a referencia do pai antes de descer
        if (z->chave < x->chave)
            x = x->fe;      //vai pra esquerda
        else
            x = x->fd;      //vai pra direita
    }
    //conecta novo nodo de baixo para cima
    z->pai = y;
 
    if (y == T->sentinela) {
        //se y nunca saiu do lugar, a arvore estava vazia e z eh a raiz
        T->raiz = z;
    } else if (z->chave < y->chave) {
        //novo nodo menor que o pai, fica a esquerda
        y->fe = z;
    } else {
        //novo nodo maior que o pai, fica a direita
        y->fd = z;
    }
    //restaura as propriedades da red-black
    redblack_fixup(T, z);
}
 
