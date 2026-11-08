#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
 
typedef struct nodo {
    int *vetor_chaves;      //vetor representando as coordenadas
    struct nodo* fe;        //ponteiro para o filho esquerdo - menor
    struct nodo* fd;        //ponteiro para o filho direito - maior ou igual
    struct nodo* pai;       //ponteiro para o nodo pai
} nodo;
 
typedef struct kd_arvore {
    nodo* raiz;
    int dimensoes;          //numero de dimensoes k da arvore
} kd_arvore;
 
typedef struct resultado_busca {
    nodo* node;             //nodo mais proximo encontrado ate agora
    double distancia;       //distancia euclidiana ate esse nodo
} resultado_busca;
 
void trocar(int **a, int **b) {
    int *aux = *a;
    *a = *b;
    *b = aux;
}
 
//ordena o subarray de vetores[l..r] pela coordenada coord (selection sort)
void ordenar(int **vetores, int coord, int l, int r) {
    for (int i = l; i <= r; i++) {
        for (int j = i + 1; j <= r; j++) {
            if (vetores[i][coord] > vetores[j][coord]) {
                trocar(&vetores[i], &vetores[j]);
            }
        }
    }
}
 
double distancia_euclidiana(int* p1, int* p2, int k) {
    double soma = 0.0;
    for (int i = 0; i < k; i++) {
        double diferenca = (double)(p1[i] - p2[i]);
        soma += (diferenca * diferenca);
    }
    return sqrt(soma);
}
 
//alocacao e inicializacao de um novo nodo da kd-tree
nodo* criar_nodo(int* vetor_chaves, int dimensoes) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    novo->vetor_chaves = (int*)malloc(dimensoes * sizeof(int));
 
    //copia as coordenadas para o novo nodo
    for (int i = 0; i < dimensoes; i++) {
        novo->vetor_chaves[i] = vetor_chaves[i];
    }
 
    novo->fe = NULL;
    novo->fd = NULL;
    novo->pai = NULL;
 
    return novo;
}
 
void inserir(kd_arvore* T, int* vetor_chaves) {
    nodo* novo = criar_nodo(vetor_chaves, T->dimensoes);
    int coord = 0;
 
    nodo* atual = T->raiz;
    nodo* pai = NULL;
    int coord_pai = 0;
 
    //busca iterativa pela posicao de insercao
    while (atual != NULL) {
        pai = atual;
        coord_pai = coord;      //salva a coordenada antes de avancar nivel
 
        //compara coordenada atual do novo vetor com a do nodo atual
        if (novo->vetor_chaves[coord] < atual->vetor_chaves[coord]) {
            atual = atual->fe;
        } else {
            atual = atual->fd;
        }
        //avanca para a prox dimensao de forma circular
        coord = (coord + 1) % T->dimensoes;
    }
    //vincula o novo nodo ao pai encontrado
    novo->pai = pai;
 
    if (pai == NULL) {
        T->raiz = novo;         //arvore estava vazia
    } else {
        if (novo->vetor_chaves[coord_pai] < pai->vetor_chaves[coord_pai]) {
            pai->fe = novo;
        } else {
            pai->fd = novo;
        }
    }
}
 
//verifica igualdade de duas chaves de k dimensoes
bool igualdade(int *chave, int *chave_busca, int k) {
    for (int i = 0; i < k; i++) {
        if (chave[i] != chave_busca[i]) {
            return false;       //retorna falso na primeira diferenca encontrada
        }
    }
    return true;
}
 
//busca exata recursiva na kd-tree: encontra o nodo igual a vetor_chaves
//funciona de forma garantida apenas em arvores construidas via 'construir'
nodo* buscar(nodo* r, int* vetor_chaves, int coord, int k) {
    //caso base: chegou em NULL (nao encontrou) ou achou o ponto
    if (r == NULL || igualdade(r->vetor_chaves, vetor_chaves, k)) {
        return r;
    }
    //compara apenas a dimensao atual e decide o caminho
    if (vetor_chaves[coord] < r->vetor_chaves[coord]) {
        return buscar(r->fe, vetor_chaves, (coord + 1) % k, k);
    }
    return buscar(r->fd, vetor_chaves, (coord + 1) % k, k);
}
 
//constroi uma kd-tree balanceada a partir de um array de vetores
//divide pelo elemento mediano de cada coordenada alternada
nodo* construir(int **vetores, int coord, int k, int l, int r) {
    if (l > r) return NULL;
 
    //calcula o indice do elemento mediano de forma segura (evita overflow)
    int meio = l + (r - l) / 2;
 
    //ordena o subarray de acordo com a coordenada atual
    ordenar(vetores, coord, l, r);
 
    //aloca e inicializa o nodo com a mediana
    nodo* novo = criar_nodo(vetores[meio], k);
 
    //constroi recursivamente a subarvore esquerda
    if (l <= meio - 1) {
        novo->fe = construir(vetores, (coord + 1) % k, k, l, meio - 1);
        if (novo->fe != NULL)
            novo->fe->pai = novo;
    }
 
    //constroi recursivamente a subarvore direita
    if (meio + 1 <= r) {
        novo->fd = construir(vetores, (coord + 1) % k, k, meio + 1, r);
        if (novo->fd != NULL)
            novo->fd->pai = novo;
    }
 
    return novo;
}
 
//busca do vizinho mais proximo (kNN com k=1)
//retorna resultado_busca com o nodo mais proximo e sua distancia
resultado_busca vizinho_mais_prox(nodo* r, int coord, int k, int* ponto, resultado_busca melhor) {
    //caso base: subarvore vazia, retorna o melhor ate agora
    if (r == NULL)
        return melhor;
 
    //se r eh folha, verifica se eh melhor que o atual melhor
    if (r->fe == NULL && r->fd == NULL) {
        double distancia = distancia_euclidiana(r->vetor_chaves, ponto, k);
        if (distancia < melhor.distancia) {
            resultado_busca novo_melhor = {r, distancia};
            return novo_melhor;
        }
        return melhor;
    }
 
    nodo* prim = NULL;
    nodo* sec = NULL;
 
    //decide o lado a explorar primeiro (o lado onde o ponto estaria na arvore)
    if (ponto[coord] < r->vetor_chaves[coord]) {
        prim = r->fe;
        sec = r->fd;
    } else {
        prim = r->fd;
        sec = r->fe;
    }
 
    //desce recursivamente pelo ramo primario
    if (prim != NULL) {
        melhor = vizinho_mais_prox(prim, (coord + 1) % k, k, ponto, melhor);
    }
 
    //na volta, verifica se o nodo atual eh melhor que o melhor encontrado
    double distancia_atual = distancia_euclidiana(r->vetor_chaves, ponto, k);
    if (distancia_atual < melhor.distancia) {
        melhor.node = r;
        melhor.distancia = distancia_atual;
    }
 
    //verifica se o hiperplano divisor pode conter um ponto ainda mais proximo
    //a distancia do ponto ao hiperplano eh apenas a diferenca na coordenada atual
    //se essa distancia for menor que a melhor distancia atual, explore o outro lado
    if (sec != NULL && fabs((double)(r->vetor_chaves[coord] - ponto[coord])) < melhor.distancia) {
        resultado_busca novo = vizinho_mais_prox(sec, (coord + 1) % k, k, ponto, melhor);
        if (novo.distancia < melhor.distancia) {
            return novo;
        }
    }
    return melhor;
}
 
void liberar_arvore(nodo* r) {
    if (r == NULL) return;
    liberar_arvore(r->fe);
    liberar_arvore(r->fd);
    free(r->vetor_chaves);
    free(r);
}
