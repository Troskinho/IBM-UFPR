#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//estrutura do nodo de uma arvore b
typedef struct nodo {
    int n;                       //numero atual de chaves armazenadas
    int* chaves;                 //vetor dinamico que armazena as chaves
    struct nodo** filhos;        //vetor dinamico que armazena os filhos
    bool eh_folha;               //verificar se o nodo eh uma folha
} nodo;

typedef struct arvoreB {
    nodo* raiz;
    int t;          //grau minimo da arvore
} arvoreB;

void verificar_alocacao(void* ptr) {
    if (ptr == NULL)
        exit(EXIT_FAILURE);
}

//aloca a memoria de um nodo na arvore B
nodo* alocar_nodo(int t) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    verificar_alocacao(novo);

    //um nodo pode ter no maximo (2t - 1) chaves e (2t) filhos
    novo->chaves = (int*)malloc(sizeof(int) * (2 * t - 1));
    verificar_alocacao(novo->chaves);
    novo->filhos = (nodo**)malloc(sizeof(nodo*) * (2 * t));
    verificar_alocacao(novo->filhos);

    novo->n = 0;
    novo->eh_folha = true;

    return novo;
}

//divide o filho i do nodo x; t eh o grau minimo da arvore
void dividir_filho(nodo* x, int i, int t) {
    //y eh o filho completamente cheio e sera dividido
    nodo* y = x->filhos[i];

    //z sera o novo irmao que fica com a metade direita de y
    nodo* z = alocar_nodo(t);
    z->eh_folha = y->eh_folha;      //z sera folha se y for folha
    z->n = t - 1;                   //z recebe t-1 chaves

    //transferencia das maiores chaves de y para z
    for (int j = 0; j < t - 1; j++) {
        z->chaves[j] = y->chaves[j + t];
    }

    //se y nao for folha, transfere tambem os filhos correspondentes
    if (y->eh_folha == false) {
        for (int j = 0; j < t; j++) {
            z->filhos[j] = y->filhos[j + t];
        }
    }
    //atualizar o contador de chaves de y
    y->n = t - 1;

    //abrir espacos nos filhos de x para acomodar o novo nodo
    //deslocar os ponteiros dos filhos de x para a direita
    for (int j = x->n; j >= i + 1; j--) {
        x->filhos[j + 1] = x->filhos[j];
    }
    //z como filho de x a direita de y
    x->filhos[i + 1] = z;

    //abre espaco nas chaves de x para alocar a mediana que vai subir
    //deslocar as chaves de x uma posicao a direita
    for (int j = x->n - 1; j >= i; j--) {
        x->chaves[j + 1] = x->chaves[j];
    }

    //chave mediana de y sobe para o pai de y
    x->chaves[i] = y->chaves[t - 1];

    //pai x tem uma chave a mais
    x->n++;
}

//insere uma chave k em um nodo x que ja se sabe que nao esta cheio
void inserir_nao_cheio(nodo* x, int k, int t) {
    //comeca na ultima chave valida do nodo
    int i = x->n - 1;

    //nodo eh uma folha
    if (x->eh_folha) {
        //encontra a posicao correta da nova chave k deslocando os maiores para a direita
        //parecido com insertion sort
        while (i >= 0 && k < x->chaves[i]) {
            x->chaves[i + 1] = x->chaves[i];    //empurra para a direita
            i--;
        }
        //insere a nova chave no espaco vazio
        x->chaves[i + 1] = k;
        //atualiza o contador de chaves do nodo
        x->n++;
    }
    //nodo eh nodo interno
    else {
        //encontra o filho correto a descer
        //navega da direita para esquerda ate achar uma chave menor que k
        while (i >= 0 && k < x->chaves[i]) {
            i--;
        }
        i++;    //ajusta o indice para apontar para o ponteiro de filho correto

        //prevencao: checa se o filho que vamos descer esta cheio
        if (x->filhos[i]->n == 2 * t - 1) {
            //filho cheio, dividimos antes de descer
            dividir_filho(x, i, t);

            //a chave do meio do filho subiu para x
            //o filho original foi separado em dois: verifica qual metade descer
            if (k > x->chaves[i]) {
                i++;        //k maior que a chave que subiu: desce para a metade direita
            }
        }
        //agora eh certeza que o filho de x nao esta cheio: chama recursivamente
        inserir_nao_cheio(x->filhos[i], k, t);
    }
}

//funcao principal de insercao da arvore b
void inserir_arvoreB(arvoreB* T, int k) {
    nodo* r = T->raiz;
    int t = T->t;   //grau minimo, usado para checar capacidade

    //verifica se a raiz esta com capacidade maxima
    if (r->n == 2 * t - 1) {
        //se a raiz encheu, a arvore cresce em altura

        //cria novo nodo que sera a nova raiz
        nodo* root = alocar_nodo(t);
        root->eh_folha = false;
        root->n = 0;            //nova raiz comeca vazia

        //raiz antiga eh rebaixada a filho da nova raiz
        root->filhos[0] = r;        //antiga raiz se torna primeiro filho de root

        T->raiz = root;          //root passa a ser a raiz da arvore T

        //dividimos o filho 0 da nova raiz (a antiga raiz cheia)
        dividir_filho(root, 0, t);

        //insercao final: a raiz agora tem 1 chave e 2 filhos nao cheios
        //a insercao de k cai na funcao inserir_nao_cheio
        inserir_nao_cheio(root, k, t);
    }
    else {
        //quando a raiz tem espaco de sobra, apenas insere no nodo nao cheio
        inserir_nao_cheio(r, k, t);
    }
}

//imprime a arvore em ordem, nivel mais profundo primeiro nas chamadas internas
void imprimir_arvoreB(nodo* x) {
    if (x == NULL) return;

    int i;
    for (i = 0; i < x->n; i++) {
        if (!x->eh_folha) {
            imprimir_arvoreB(x->filhos[i]);
        }
        printf("%d ", x->chaves[i]);
    }
    if (!x->eh_folha) {
        imprimir_arvoreB(x->filhos[i]);
    }
}

//libera a memoria dos filhos, chaves e toda a arvore
void liberar_arvoreB(nodo* x) {
    if (x == NULL) return;

    if (!x->eh_folha) {
        for (int i = 0; i <= x->n; i++) {
            liberar_arvoreB(x->filhos[i]);
        }
    }
    free(x->chaves);
    free(x->filhos);
    free(x);
}
