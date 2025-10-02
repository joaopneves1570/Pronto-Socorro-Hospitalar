#include "triagem.h"
#define TAM_MAX 50

typedef struct no_ NO;

struct no_{
    NO* prox;
    PACIENTE* pac;
};

struct triagem_{
    NO* inicio;
    NO* fim;
    int tamanho;
};

TRIAGEM* triagem_criar(){
    TRIAGEM* tri = (TRIAGEM* )malloc(sizeof(TRIAGEM));
    if (tri != NULL){
        tri->inicio = NULL;
        tri->fim = NULL;
        tri->tamanho = 0;
    }

    return tri;
}

bool triagem_inserir(TRIAGEM* tri, PACIENTE* pac){
    if ((tri != NULL) && (!triagem_cheia(tri))){
        NO* novo = (NO*)malloc(sizeof(NO));

        if (triagem_vazia(tri)){
            tri->inicio = novo;
        } else {
            tri->fim->prox = novo;
        }

        novo->pac = pac;
        novo->prox = NULL;
        tri->fim = novo;
        tri->tamanho++;
        return true;
    }

    return false;
}

PACIENTE* triagem_remover(TRIAGEM* tri){
    if ((tri != NULL) && (!triagem_vazia(tri))){
        NO* atual = tri->inicio;
        PACIENTE* pac = atual->pac;

        tri->inicio = atual->prox;
        atual->prox = NULL;
        free(atual); atual = NULL;

        if (tri->tamanho == 1)
            tri->fim = NULL;

        tri->tamanho--;
        return pac;
    }

    return NULL;
}

bool triagem_cheia(TRIAGEM* tri){
    if (tri != NULL){
        return (tri->tamanho == TAM_MAX);
    }
    return false;
    
}

bool triagem_vazia(TRIAGEM* tri){
    if (tri != NULL){
        return (tri->inicio == NULL);
    }
    return true;
}


void triagem_apagar(TRIAGEM** tri) {
    if (tri == NULL || *tri == NULL) {
        return;
    }

    NO* atual = (*tri)->inicio;
    while (!triagem_vazia(*tri)) {
        NO* prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(*tri);
    *tri = NULL;
}

void triagem_mostrar(TRIAGEM* tri){
    if (tri != NULL && !triagem_vazia(tri)){
        NO* atual = tri->inicio;
        PACIENTE* pac;
        for (int i = 0; i < tri->tamanho; i++){
            pac = atual->pac;
            paciente_imprimir(pac);
            atual = atual->prox;
        }
    }
}