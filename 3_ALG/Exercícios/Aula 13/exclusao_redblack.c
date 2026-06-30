#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum { VERMELHO, PRETO } Cor;

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

void rotacao_direita(arvore* T, nodo* y) {
    nodo* x = y->fe;
    y->fe = x->fd;
    if (x->fd != T->sentinela) {
        x->fd->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == T->sentinela) {
        T->raiz = x;
    }
    else if (y == y->pai->fd) {
        y->pai->fd = x;
    }
    else {
        y->pai->fe = x;
    }
    x->fd = y;
    y->pai = x;
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

//substitui a subarvore com raiz em u pela subarvore com raiz em v
void transplantar_redblack(arvore* T, nodo* u, nodo* v) {
    //atualiza o ponteiro do pai de u para apontar para v
    if (u->pai == T->sentinela) {
        T->raiz = v;    //se u nao tem pai, ele era a raiz; v se torna a nova raiz
    }
    else if (u == u->pai->fe) {
        u->pai->fe = v; //u era filho esquerdo; pai adota v na esquerda
    }
    else {
        u->pai->fd = v; //u era filho direito; pai adota v na direita
    }
    //atualiza o ponteiro de pai de v para o antigo pai de u
    v->pai = u->pai;
}

//retorna o nodo com menor chave na subarvore com raiz em x
nodo* minimo(arvore* T, nodo* x) {
    while (x->fe != T->sentinela) {
        x = x->fe;
    }
    return x;
}

//restaura as propriedades rubro-negras apos uma exclusao
void delete_fixup(arvore* T, nodo* x) {
    nodo* z;    //z sera sempre o irmao de x

    while (x != T->raiz && x->cor == PRETO) {
        //x eh um filho esquerdo
        if (x == x->pai->fe) {
            z = x->pai->fd;         //irmao z esta na direita

            //caso 1: irmao z eh vermelho
            if (z->cor == VERMELHO) {
                z->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacao_esquerda(T, x->pai);    //rotaciona em torno do pai de x
                z = x->pai->fd;                 //atualiza o irmao apos a rotacao
            }
            //caso 2: irmao z eh preto e seus dois filhos sao pretos
            if (z->fe->cor == PRETO && z->fd->cor == PRETO) {
                z->cor = VERMELHO;
                x = x->pai;
            }
            else {
                //caso 3: irmao z eh preto, filho direito de z eh preto
                if (z->fd->cor == PRETO) {
                    z->fe->cor = PRETO;
                    z->cor = VERMELHO;
                    rotacao_direita(T, z);
                    z = x->pai->fd;
                }
                //caso 4: irmao z eh preto e filho direito de z eh vermelho
                z->cor = x->pai->cor;
                x->pai->cor = PRETO;
                z->fd->cor = PRETO;
                rotacao_esquerda(T, x->pai);
                x = T->raiz;        //forca saida do loop
            }
        }
        //x eh um filho direito - simetrico
        else {
            z = x->pai->fe;

            //caso 1 espelhado: irmao z eh vermelho
            if (z->cor == VERMELHO) {
                z->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacao_direita(T, x->pai);
                z = x->pai->fe;
            }
            //caso 2 espelhado: irmao z eh preto e seus dois filhos sao pretos
            if (z->fd->cor == PRETO && z->fe->cor == PRETO) {
                z->cor = VERMELHO;
                x = x->pai;
            }
            else {
                //caso 3 espelhado: irmao z eh preto, filho esquerdo de z eh preto
                if (z->fe->cor == PRETO) {
                    z->fd->cor = PRETO;
                    z->cor = VERMELHO;
                    rotacao_esquerda(T, z);
                    z = x->pai->fe;
                }
                //caso 4 espelhado: irmao z eh preto, filho esquerdo de z eh vermelho
                z->cor = x->pai->cor;
                x->pai->cor = PRETO;
                z->fe->cor = PRETO;
                rotacao_direita(T, x->pai);
                x = T->raiz;        //forca saida do loop
            }
        }
    }
    x->cor = PRETO;
}

//remove o nodo z da arvore rubro-negra T
void excluir_redblack(arvore* T, nodo* z) {
    nodo* y = z;            //y eh o nodo que sera fisicamente removido
    Cor cor_original = y->cor;  //guardamos a cor para checar violacao de propriedades
    nodo* x;                //x vai ocupar o lugar original de y

    //caso 1: nodo z nao tem filho esquerdo
    if (z->fe == T->sentinela) {
        x = z->fd;
        transplantar_redblack(T, z, z->fd);
    }
    //caso 2: nodo z nao tem filho direito
    else if (z->fd == T->sentinela) {
        x = z->fe;
        transplantar_redblack(T, z, z->fe);
    }
    //caso 3: nodo z tem os dois filhos
    else {
        //encontrar o sucessor de z (minimo da subarvore direita)
        y = minimo(T, z->fd);
        cor_original = y->cor;  //a cor do sucessor importa

        x = y->fd;  //x eh o unico filho que o sucessor y pode ter (somente a direita)

        if (y != z->fd) {
            //sucessor nao eh filho direto de z: transplanta y para fora do lugar
            transplantar_redblack(T, y, y->fd);
            y->fd = z->fd;
            y->fd->pai = y;
        } else {
            //sucessor eh filho direto de z: x aponta para y para ajuste de pai no fixup
            x->pai = y;
        }
        //substitui z por y na arvore
        transplantar_redblack(T, z, y);
        y->fe = z->fe;
        y->fe->pai = y;
        //o sucessor y assume a cor que z possuia
        y->cor = z->cor;
    }
    //se o nodo fisicamente removido era preto, a altura preta foi alterada
    if (cor_original == PRETO) {
        delete_fixup(T, x);
    }
    //libera a memoria do nodo desconectado da arvore
    free(z);
}

void liberar_rb(nodo* x, nodo* sentinela) {
    if (x == sentinela) return;
    liberar_rb(x->fe, sentinela);
    liberar_rb(x->fd, sentinela);
    free(x);
}
