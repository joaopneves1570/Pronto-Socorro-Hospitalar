#include "../include/historico.h"

struct historico_{
    char hist[10][100];
    int tamanho;
};

HISTORICO* historico_criar(void){
    HISTORICO* hist = (HISTORICO*)malloc(sizeof(HISTORICO));
    if (hist != NULL){
        hist->tamanho = 0;
    }

    return hist;
}

int historico_tamanho(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho;
    }

    return -1;
}

bool historico_cheio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == 10;
    }

    return false;
}

bool historico_vazio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == 0;
    }

    return true;;
}

bool historico_inserir(HISTORICO* hist, char texto[]){
    if (hist != NULL && !historico_cheio(hist)){
        strcpy(hist->hist[hist->tamanho], texto);
        hist->tamanho++;
        return true;
    }

    return false;
}

char* historico_remover(HISTORICO* hist){
    if (hist != NULL && !historico_vazio(hist)){
        hist->tamanho--;
        return hist->hist[hist->tamanho];
    }

    return NULL;
}

bool historico_consultar(HISTORICO* hist, char* texto){
    if (hist != NULL){
        for (int i = 0; i < hist->tamanho; i++){
            if (strcmp(hist->hist[i], texto) == 0){
                return true;
            }
        }
    }
    return false;
}

void historico_imprimir(HISTORICO* hist){
    if (hist != NULL){
        for (int i = 0; i < hist->tamanho; i++){
            printf("%S\n", hist->hist[i]);
        }
    }
}

void historico_apagar(HISTORICO** hist){
    if (hist != NULL && *hist != NULL){
        free(*hist);
        *hist = NULL;
    }
}

