#include "../include/historico.h"
#include <stdio.h>
#define TAM_MAX 10

typedef struct no_ NO;

struct no_{
    NO* anterior;
    char procedimento[100];
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

bool historico_inserir(HISTORICO* hist, char procedimento[]){
    if (hist != NULL && !historico_cheio(hist)){
        NO* novo = (NO* )malloc(sizeof(NO));
        if (novo != NULL){
            snprintf(novo->procedimento, 100, "%s", procedimento);
            novo->anterior = hist->topo;
            hist->topo = novo;
            hist->tamanho++;

            return true;
        }
    }
    return false;
}
char* historico_remover(HISTORICO* hist){
    if (hist != NULL && !historico_vazio(hist)){
        NO* aux = hist->topo;

        char* procedimento = calloc(100, sizeof(char));

        strcpy(procedimento, hist->topo->procedimento);

        hist->topo = aux->anterior;
        aux->anterior = NULL;
        free(aux); aux = NULL;

        hist->tamanho--;

        return procedimento;
    }
    return NULL;
}

bool historico_consultar(HISTORICO* hist, char* procedimento){
    if (hist != NULL && !historico_vazio(hist)){

        strcpy(procedimento, hist->topo->procedimento);

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

int historico_tamanho(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho;
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

void historico_imprimir(HISTORICO* hist){
    if (hist != NULL){
        if (historico_vazio(hist))
        {
            printf("O histórico do paciente está vazio.\n");
            return;
        }
        for (NO* no = hist->topo; no != NULL; no = no->anterior)
        {
            printf("- %s\n", no->procedimento);
        }
    }
}

