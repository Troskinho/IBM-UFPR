#include "trabalho.h"
#include <string.h>

int comparacao(const void *a, const void *b) {
    int valorA = *(const int*)a;
    int valorB = *(const int*)b;

    if (valorA < valorB) return -1;
    else if (valorA > valorB) return 1;
    else return 0;
}

int gerarRuns(const char* entrada) {
    FILE* input = fopen(entrada, "r");
    if (!input) {
        perror("Erro ao abrir");
        exit(1);
    }

    int* buffer = malloc(sizeof(int) * Maximo_Linhas);
    int contaruns = 0, totalDeRUns = 0;

    while (1) {
        contaruns = 0;
        while (contaruns < Maximo_Linhas && fscanf(input, "%d", &buffer[contaruns]) == 1) {
            contaruns++;
        }

        if (contaruns == 0) break;

        qsort(buffer, contaruns, sizeof(int), comparacao);

        char titulo[20];
        sprintf(titulo, "run%d.txt", totalDeRUns);
        FILE* run = fopen(titulo, "w");
        for (int i = 0; i < contaruns; i++)
            fprintf(run, "%d\n", buffer[i]);
        fclose(run);

        totalDeRUns++;
    }

    fclose(input);
    free(buffer);
    return totalDeRUns;
}

int valores[K];
int origens[K];
FILE* arquivos[K];
int tamanho_heap = 0;

void trocar(int i, int j) {
    int tempValores = valores[i];
    valores[i] = valores[j];
    valores[j] = tempValores;

    int tempOrigens = origens[i];
    origens[i] = origens[j];
    origens[j] = tempOrigens;
}

void minHeapfy(int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < tamanho_heap && valores[esq] < valores[menor]) menor = esq;
    if (dir < tamanho_heap && valores[dir] < valores[menor]) menor = dir;

    if (menor != i) {
        trocar(i, menor);
        minHeapfy(menor);
    }
}

void constroiHeap() {
    for (int i = tamanho_heap / 2 - 1; i >= 0; i--) {
        minHeapfy(i);
    }
}

void intercala(int totalderuns) {
    int rodada = 0;
    int resto = totalderuns;
    char saida[30] = "saida.txt";

    while (resto > 1) {
        int novas = 0;
        for (int i = 0; i < resto; i += K) {
            int atual_k = (i + K <= resto) ? K : (resto - i);
            tamanho_heap = 0;
            char titulosaida[30];
            sprintf(titulosaida, "run_temp_%d_%d.txt", rodada, novas);
            FILE* saida = fopen(titulosaida, "w");

            for (int j = 0; j < atual_k; j++) {
                char nome[20];
                sprintf(nome, "run%d.txt", i + j);
                arquivos[j] = fopen(nome, "r");
                if (fscanf(arquivos[j], "%d", &valores[j]) == 1) {
                    origens[j] = j;
                    tamanho_heap++;
                }
            }
            constroiHeap();

            while (tamanho_heap > 0) {
                int menor = valores[0];
                int origem = origens[0];
                fprintf(saida, "%d\n", menor);

                if (fscanf(arquivos[origem], "%d", &valores[0]) == 1) {
                    origens[0] = origem;
                } else {
                    valores[0] = valores[tamanho_heap - 1];
                    origens[0] = origens[tamanho_heap - 1];
                    tamanho_heap--;
                }
                minHeapfy(0);
            }

            for (int j = 0; j < atual_k; j++) fclose(arquivos[j]);
            fclose(saida);
            novas++;
        }

        for (int i = 0; i < novas; i++) {
            char antigo[30], novo[20];
            sprintf(antigo, "run_temp_%d_%d.txt", rodada, i);
            sprintf(novo, "run%d.txt", i);
            rename(antigo, novo);
        }

        rodada++;
        resto = novas;
    }

    rename("run0.txt", saida);
    printf("ordenado %s\n", saida);
}

int main() {
    int total_runs = gerarRuns("entrada.txt");
    intercala(total_runs);
    return 0;
}
