#include "historico.h"
#define TAM_MAX 10

typedef struct no_ NO;

struct no_{
    NO* anterior;
    char texto[100];
};

struct historico_{
    NO* topo;
    int tamanho;
};

HISTORICO* historico_criar(void){
    HISTORICO* hist = (HISTORICO* )malloc(sizeof(HISTORICO));
    if (hist != NULL){
        hist->tamanho = 0;
        hist->topo = NULL;
        return hist;
    }

    return NULL;
}

bool historico_inserir(HISTORICO* hist, char texto[]){
    if (hist != NULL && !historico_cheio(hist)){
        NO* novo = (NO* )malloc(sizeof(NO));
        if (novo != NULL){
            strcpy(novo->texto, texto);
            novo->anterior = hist->topo;
            hist->topo = novo;
            hist->tamanho++;

            return true;
        }
    }
    return false;
}
bool historico_retirar(HISTORICO* hist, char* texto){
    if (hist != NULL && !historico_vazio(hist)){
        NO* aux = hist->topo;

        strcpy(texto, hist->topo->texto);

        hist->topo = aux->anterior;
        aux->anterior = NULL;
        free(aux); aux = NULL;

        hist->tamanho--;

        return true;
    }
    return false;
}

bool historico_consultar(HISTORICO* hist, char* texto){
    if (hist != NULL && !historico_vazio(hist)){

        strcpy(texto, hist->topo->texto);

        return true;
    }
    return false;
}
bool historico_cheio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == TAM_MAX;
    }
    return false;
}
bool historico_vazio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == 0;
    }
    return false;
}

void historico_apagar(HISTORICO** hist){
    if ((*hist) != NULL && (!historico_vazio(*hist))){
        while ((*hist)->topo != NULL){
            NO* aux = (*hist)->topo;
            (*hist)->topo = aux->anterior;
            aux->anterior = NULL;
            free(aux); aux = NULL;
        }
    }
    free(*hist); (*hist) = NULL;
}

