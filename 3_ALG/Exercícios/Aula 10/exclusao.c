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

void dividir_filho(nodo* x, int i, int t) {
    nodo* y = x->filhos[i];

    nodo* z = alocar_nodo(t);
    z->eh_folha = y->eh_folha;
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++) {
        z->chaves[j] = y->chaves[j + t];
    }
    if (y->eh_folha == false) {
        for (int j = 0; j < t; j++) {
            z->filhos[j] = y->filhos[j + t];
        }
    }
    y->n = t - 1;

    for (int j = x->n; j >= i + 1; j--) {
        x->filhos[j + 1] = x->filhos[j];
    }
    x->filhos[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--) {
        x->chaves[j + 1] = x->chaves[j];
    }
    x->chaves[i] = y->chaves[t - 1];

    x->n++;
}

void inserir_nao_cheio(nodo* x, int k, int t) {
    int i = x->n - 1;

    if (x->eh_folha) {
        while (i >= 0 && k < x->chaves[i]) {
            x->chaves[i + 1] = x->chaves[i];
            i--;
        }
        x->chaves[i + 1] = k;
        x->n++;
    } else {
        while (i >= 0 && k < x->chaves[i]) {
            i--;
        }
        i++;
        if (x->filhos[i]->n == 2 * t - 1) {
            dividir_filho(x, i, t);
            if (k > x->chaves[i]) {
                i++;
            }
        }
        inserir_nao_cheio(x->filhos[i], k, t);
    }
}

void inserir_arvoreB(arvoreB* T, int k) {
    nodo* r = T->raiz;
    int t = T->t;

    if (r->n == 2 * t - 1) {
        nodo* root = alocar_nodo(t);
        root->eh_folha = false;
        root->n = 0;
        root->filhos[0] = r;
        T->raiz = root;
        dividir_filho(root, 0, t);
        inserir_nao_cheio(root, k, t);
    } else {
        inserir_nao_cheio(r, k, t);
    }
}

//funcao auxiliar para achar o predecessor (valor maximo) na subarvore a esquerda de x
int obter_predecessor(nodo* x) {
    nodo* atual = x;
    while (!atual->eh_folha) {
        atual = atual->filhos[atual->n]; //vai sempre para o ultimo filho a direita
    }
    return atual->chaves[atual->n - 1]; //retorna a maior chave da folha
}

//funcao auxiliar para achar o sucessor (valor minimo) na subarvore a direita de x
int obter_sucessor(nodo* x) {
    nodo* atual = x;
    while (!atual->eh_folha) {
        atual = atual->filhos[0]; //vai sempre para o primeiro filho a esquerda
    }
    return atual->chaves[0]; //retorna a menor chave da folha
}

//funde x->filhos[i], a chave x->chaves[i] e x->filhos[i+1] em um unico nodo
void merge(nodo* x, int i, int t) {
    nodo* filho_esq = x->filhos[i];
    nodo* filho_dir = x->filhos[i + 1];

    //a chave i do pai desce e se torna a chave mediana do filho esquerdo
    filho_esq->chaves[t - 1] = x->chaves[i];

    //as chaves do filho direito sao copiadas para o filho esquerdo
    for (int j = 0; j < t - 1; j++) {
        filho_esq->chaves[j + t] = filho_dir->chaves[j];
    }
    //se nao forem folhas, os ponteiros de filhos tambem sao copiados
    if (!filho_esq->eh_folha) {
        for (int j = 0; j < t; j++) {
            filho_esq->filhos[j + t] = filho_dir->filhos[j];
        }
    }
    //filho esquerdo agora esta completamente cheio
    filho_esq->n = 2 * t - 1;

    //remove a chave i e o ponteiro i+1 do pai, deslocando o restante
    for (int j = i + 1; j < x->n; j++) {
        x->chaves[j - 1] = x->chaves[j];
    }
    for (int j = i + 2; j <= x->n; j++) {
        x->filhos[j - 1] = x->filhos[j];
    }
    x->n--;         //pai perdeu uma chave

    //libera o nodo direito, que foi totalmente absorvido pelo esquerdo
    free(filho_dir->chaves);
    free(filho_dir->filhos);
    free(filho_dir);
}

//empresta uma chave do irmao esquerdo (filhos[i-1]) para filhos[i]
void emprestar_esquerda(nodo* x, int i) {
    nodo* filho = x->filhos[i];
    nodo* irmao_esq = x->filhos[i - 1];

    //abre espaco no inicio do filho, empurrando tudo para a direita
    for (int j = filho->n - 1; j >= 0; j--) {
        filho->chaves[j + 1] = filho->chaves[j];
    }
    if (!filho->eh_folha) {
        for (int j = filho->n; j >= 0; j--) {
            filho->filhos[j + 1] = filho->filhos[j];
        }
    }

    //a chave do pai (i-1) desce para o inicio do filho
    filho->chaves[0] = x->chaves[i - 1];

    //o ultimo filho do irmao esquerdo passa a ser o primeiro filho do nodo
    if (!filho->eh_folha) {
        filho->filhos[0] = irmao_esq->filhos[irmao_esq->n];
    }

    //a ultima chave do irmao esquerdo sobe para a posicao i-1 do pai
    //(mesma posicao que desceu, nao i+1)
    x->chaves[i - 1] = irmao_esq->chaves[irmao_esq->n - 1];

    //ajuste dos contadores
    filho->n += 1;
    irmao_esq->n -= 1;
}

//empresta uma chave do irmao direito (filhos[i+1]) para filhos[i]
void emprestar_direita(nodo* x, int i) {
    nodo* filho = x->filhos[i];
    nodo* irmao_dir = x->filhos[i + 1];

    //a chave do pai desce para o final do filho
    filho->chaves[filho->n] = x->chaves[i];

    //o primeiro filho do irmao direito passa a ser o ultimo filho do nodo
    if (!filho->eh_folha) {
        filho->filhos[filho->n + 1] = irmao_dir->filhos[0];
    }

    //a primeira chave do irmao direito sobe para o pai
    x->chaves[i] = irmao_dir->chaves[0];

    //abre espaco no irmao direito, empurrando tudo para a esquerda
    for (int j = 1; j < irmao_dir->n; j++) {
        irmao_dir->chaves[j - 1] = irmao_dir->chaves[j];
    }
    if (!irmao_dir->eh_folha) {
        for (int j = 1; j <= irmao_dir->n; j++) {
            irmao_dir->filhos[j - 1] = irmao_dir->filhos[j];
        }
    }

    //ajuste dos contadores
    filho->n += 1;
    irmao_dir->n -= 1;
}

//garante que x->filhos[i] tenha pelo menos t chaves antes de descer nele
void preencher(nodo* x, int i, int t) {
    //caso 3a: irmao da esquerda tem chaves sobrando
    if (i > 0 && x->filhos[i - 1]->n >= t) {
        emprestar_esquerda(x, i);
    }
    //caso 3a simetrico: irmao da direita tem chaves sobrando
    else if (i < x->n && x->filhos[i + 1]->n >= t) {
        emprestar_direita(x, i);
    }
    //caso 3b: nenhum irmao tem chave sobrando, faz o merge
    else {
        if (i != x->n) {
            //nao eh o ultimo filho: funde com o irmao da direita
            merge(x, i, t);
        } else {
            //eh o ultimo filho: funde com o irmao da esquerda
            merge(x, i - 1, t);
        }
    }
}

//remove a chave k da subarvore com raiz em x
void excluir_arvoreB(arvoreB* T, nodo* x, int k) {
    int i = 0;
    int t = T->t;

    //busca a posicao da chave no nodo atual
    while (i < x->n && k > x->chaves[i]) {
        i++;
    }

    //chave encontrada neste nodo
    if (i < x->n && k == x->chaves[i]) {
        //caso 1: o nodo encontrado eh folha
        if (x->eh_folha) {
            //remove k de x, deslocando as chaves a direita de k
            for (int j = i + 1; j < x->n; j++) {
                x->chaves[j - 1] = x->chaves[j];
            }
            x->n--;
            return;
        }
        //caso 2: chave em um nodo interno
        else {
            //caso 2a: filho esquerdo com chaves de sobra (>= t)
            if (x->filhos[i]->n >= t) {
                int pred = obter_predecessor(x->filhos[i]);
                x->chaves[i] = pred;
                //remove o predecessor da subarvore esquerda (desce no FILHO, nao na chave)
                excluir_arvoreB(T, x->filhos[i], pred);
            }
            //caso 2b: filho direito com chaves de sobra (>= t)
            else if (x->filhos[i + 1]->n >= t) {
                int suc = obter_sucessor(x->filhos[i + 1]);
                x->chaves[i] = suc;
                excluir_arvoreB(T, x->filhos[i + 1], suc);
            }
            //caso 2c: nenhum dos dois filhos tem chave de sobra, faz o merge
            else {
                merge(x, i, t);
                //apos o merge, k esta no filho esquerdo fundido (filhos[i])
                excluir_arvoreB(T, x->filhos[i], k);

                //se x era a raiz e ficou vazia, o filho fundido vira a nova raiz
                if (x->n == 0 && T->raiz == x) {
                    T->raiz = x->filhos[0];
                    free(x->chaves);
                    free(x->filhos);
                    free(x);
                }
            }
        }
    }
    //chave nao esta neste nodo
    else {
        if (x->eh_folha) {
            return;         //a chave nao existe na arvore
        }
        //descida segura: garante que o filho tenha pelo menos t chaves antes de descer
        if (x->filhos[i]->n == t - 1) {
            preencher(x, i, t);
            //apos merge/emprestimo, o indice do filho correto pode ter mudado
            if (i > x->n) {
                i--;
            }
        }
        excluir_arvoreB(T, x->filhos[i], k);

        //caso em que x era a raiz e o merge/preencher a esvaziou
        if (x->n == 0 && T->raiz == x) {
            T->raiz = x->filhos[0];
            free(x->chaves);
            free(x->filhos);
            free(x);
        }
    }
}


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
