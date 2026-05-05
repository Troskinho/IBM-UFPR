#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct porcao { // nesse caso porção = nodo
    int id;
    int porcoes;
    struct porcao* prox;
}porcao;

typedef struct lista {
    porcao* atual;
    int tamanho;
    int prox_id;
} lista;

void inicializa_lista(lista* lista) {
    lista->atual = NULL;
    lista->tamanho = 0;
    lista->prox_id = 1;
}

porcao* criar_porcao(int id, int porcoes) {
    porcao* nova = (porcao*)malloc(sizeof(porcao));
    nova->id = id;
    nova->porcoes = porcoes;
    nova->prox = NULL; // não tem uma porção depois da ultima criada
    return nova;
}

void inserir_lista(lista *lista, int porcoes) {
    porcao* nova = criar_porcao(lista->prox_id++, porcoes);

    if (lista->atual == NULL) {
        nova->prox = nova;
        lista->atual = nova;
    } else {
        porcao *temp = lista->atual;
        
        while (temp->prox != lista->atual){ //procura sempre o ultimo elemento da lista, para que seja adicionado um elemento novo ao final dela
            temp = temp->prox;
        }

        temp->prox = nova;
        nova->prox = lista->atual;
    }
    lista->tamanho++;
    return;
}

void remover_porcao(lista *lista, porcao *anterior) {
    porcao* remover = lista->atual;

    if (remover->prox == remover) {
        lista->atual = NULL;
    } else {
        anterior->prox = remover->prox;
        lista->atual = remover->prox;
    }
    free(remover);
    lista->tamanho--;
    return;
}

void executar_volta(lista *lista) {

    if (lista->atual == NULL) {
        printf("Não há mais porções na fila\n");
        return;
    }
    
    porcao *inicio = lista->atual;
    porcao *anterior = NULL;

    do {
        int consumo =  rand() % 3; //consmo de 0 a 2 peças de sushi
        
        lista->atual->porcoes -= consumo;

        if (lista->atual->porcoes <= 0) {
            if (anterior == NULL) {
                porcao* temp = lista->atual;
                while (temp->prox != lista->atual)
                    temp = temp->prox;
                anterior = temp;
            }

            remover_porcao(lista, anterior);

            if (lista->atual == NULL) {
                printf("Todas as porções acabaram\n");
                return;
            }

            inicio = lista->atual;
        } else {
            anterior = lista->atual;
            lista->atual = lista->atual->prox;
        }
    } while (lista->atual != inicio);
}

int main() {
    lista lista;
    inicializa_lista(&lista);

    srand(time(NULL));

    int unidades, opcao;

    do {
        printf("1 - Cadastrar novas porções\n");
        printf("2 - Executar uma volta completa\n");
        printf("0 - Encerrar o programa\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Inserir a quantia de unidades para cadastro:\n");
                scanf("%d", &unidades);
                inserir_lista(&lista, unidades);
                break;

            case 2:
                executar_volta(&lista);
                break;

            case 0:
                break;
        }
    } while ((opcao != 0));
    return 0;
}