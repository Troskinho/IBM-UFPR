#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

typedef struct pessoa {
    unsigned short int idade;
    char nome[MAX_NOME];
    int cpf;
} pessoa;

typedef struct dicionario_pessoa {
    struct pessoa* pessoas;
    int tamanho;
    int capacidade;
} dicionario_pessoa;

dicionario_pessoa* criar_dicionario(int capacidade) {

    //aloca memoria para criar o dicionario - vetor
    dicionario_pessoa* dict = (dicionario_pessoa*)malloc(sizeof(dicionario_pessoa));
    if (dict == NULL) //se ele nao existe, retorna nulo
        return NULL;

    //aloca memoria para o vetor de pessoas e ir adicionando os dicionarios pessoas
    dict->pessoas = (pessoa*)malloc(capacidade * sizeof(pessoa));
    if (dict->pessoas == NULL) {
        free(dict);
        return NULL; //se nao tem pessoas no vetor, libera a memoria do dicionario e retorna nulo
    }

    //se as verificacoes passarem, cria o dicionario e inicializa ele em zero 
    dict->tamanho = 0;
    dict->capacidade = capacidade;
    return dict;
}

//busca uma pessoa passando o dicionario e uma variavel de busca como parametros
//poderiamos fazer outra funcao com outra variavel de bsuca tbm 
int buscar_pessoa(dicionario_pessoa* dict, int cpf_busca) {

    for (int i = 0; i < dict->tamanho; i++) {
        if (dict->pessoas[i].cpf, cpf_busca ) { //copia os cpf para ver se sao iguais
            return i;
        }
    }
    return -1; //caso nao encontre
}

//insere uma pessoa com struct nova
int inserir_pessoa(dicionario_pessoa* dict, pessoa* nova) {

    //confere se o dicionario já esta em sua capacidade maxima
    if (dict->tamanho >= dict->capacidade) {
        printf("Dicionário cheio!");
        return 0;
    }

    if (buscar_pessoa(dict, nova->cpf) != -1) {
        printf("Pessoa já está presente no dicionário");
        return 0;
    }

    dict->pesssoas[dict->tamanho] = *nova;
    dict->tamanho++;
    return 1;
}

//libera a memoria alocada para o dicionario
void liberar_dicionario(dicionario_pessoa* dict) {

    if (dict != NULL) {
        free(dict->pessoas);
        free(dict);
    }
}

int remover_pessoa(dicionario_pessoa* dict, int cpf_remove) {

    int indice = buscar_pessoa(dict, cpf_remove);

    if (indice == -1){
        printf("cpf não encontrado para remoção");
        return 0;
    }

    dict->pessoas[indice] = dict->pessoa[dict->tamanho - 1];
    dict->tamanho--;
    return 1;
}
