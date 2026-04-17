#include "gbv.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>

static char buffer1[BUFFER_SIZE]; //buffer global para leitura/escrita
static char buffer2[BUFFER_SIZE]; //segundo buffer global para operações 

//estrutura interna do superbloco 
typedef struct {
    int count;                  //numero de documentos
    long offset_diretorio;;     //offset do diretorio no arquivo
} superbloco;

//le o superbloco do inicio do arquivo
static int ler_suplerbloco(FILE *file, superbloco *sb) {
    rewind(file);
    if (fread(sb, sizeof(superbloco), 1, file) != 1) {
        return -1; //erro ao ler o superbloco
    }
    return 0;
}

//grava o superbloco no inicio do arquivo
static int escrever_superbloco(FILE *file, const superbloco *sb) {
    rewind(file);
    if (fwrite(sb, sizeof(superbloco), 1, file) != 1) {
        return -1;//erro ao escrever o superbloco
    }
    return 0;
}

static int salvar_biblioteca(FILE *file, Library *library, long offset_diretorio) { 
    //vai ao inicio do diretorio
    if (fseek(file, offset_diretorio, SEEK_SET) != 0) {
        return -1;
    }
}