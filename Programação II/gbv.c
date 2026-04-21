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

static int salvar_biblioteca(FILE *file, Library *lib, long offset_diretorio) { 
    //vai ao inicio do diretorio
    if (fseek(file, offset_diretorio, SEEK_SET) != 0) {
        return -1;
    }

    for (int i= 0; i < lib->count; i++) {
        if (fwrite(&lib->docs[i], sizeof(Document), 1, file) != 1) {
            return -1; 
        }

        superbloco sb;
        sb.count = lib->count;
        sb.offset_diretorio = offset_diretorio;
        if (escrever_superbloco(file, &sb) != 0) {
            return -1;  
        }
    }
    
    return 0;
}

int gbv_create(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return -1;

    superbloco sb;
    sb.count = 0;
    sb.offset_diretorio = sizeof(superbloco); //diretorio logo apos o superbloco
    if (fwrite(&sb, sizeof(superbloco), 1, file) != 1) {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;   
}

int gbv_open(Library *lib, const char *filename) {
    lib->docs = NULL;
    lib->count = 0;

    FILE *file = fopen(filename, "rb");
    if (!file) {    
        return gbv_create(filename); //cria nova biblioteca se o arquivo nao existir
    }

    superbloco sb;
    if (ler_superbloco(file, &sb) != 0) {
        fclose(file);
        return -1; 
    }

    lib->count = sb.count;

    if (sb.count == 0) {
        fclose(file);
        return 0; //biblioteca vazia
    }

    lib->docs = malloc(sb.count * sizeof(Document));
    if (!lib->docs) {
        fclose(file);
        return -1; //erro de alocacao 
    }

    if (fseek(file, sb.offset_diretorio, SEEK_SET) != 0) {
        free(lib->docs);
        fclose(file);
        return -1; //erro ao posicionar no diretorio
    }

    for (int i = 0; i < sb.count; i++) {
        if (fread(&lib->docs[i], sizeof(Document), 1, file) != 1) {
            free(lib->docs);
            fclose(file);
            return -1; //erro ao ler o diretorio
        }
    }

    fclose(file);
    return 0;
}

int gbv_add(Library *lib, const char *archive, const char *docname){
    FILE *src = fopen(docname, "rb");
    if (!src) {
        printf("Erro: não foi possível abrir '%s'\n", docname);
        return -1;
    }
    fseek(src, 0, SEEK_END);
    long doc_size = ftell(src);
    rewind(src);

    FILE *lib_f = fopen(archive, "r+b");
    if (!lib_f) {
        fclose(src);
        return -1;
    }
    superbloco sb;
    if (ler_superbloco(lib_f, &sb) != 0) {
        fclose(src);
        fclose(lib_f);
        return -1;
    }

    const char *doc_basename = strrchr(docname, '/');
    doc_basename = (doc_basename) ? doc_basename + 1 : docname;

    int existing_idx = -1;
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, doc_basename) == 0){
            existing_idx = i;
            break;  
        }
    }
    long new_offset = sb.offset_diretorio;

    if (existing_idx >= 0) {
    
}
