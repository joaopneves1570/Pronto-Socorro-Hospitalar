#include "../include/fila.h"
#include <stdio.h>
#define TAM_MAX 50

typedef struct no_ NO;

struct no_{
    NO* prox;
    PACIENTE* pac;
};

struct fila_{
    NO* inicio;
    NO* fim;
    int tamanho;
};

FILA* fila_criar(){
    FILA* fila = (FILA* )malloc(sizeof(FILA));
    if (fila != NULL){
        fila->inicio = NULL;
        fila->fim = NULL;
        fila->tamanho = 0;
    }

    return fila;
}

bool fila_inserir(FILA* fila, PACIENTE* pac){
    if ((fila != NULL) && (!fila_cheia(fila))){
        NO* novo = (NO*)malloc(sizeof(NO));

        if (fila_vazia(fila)){
            fila->inicio = novo;
        } else {
            fila->fim->prox = novo;
        }

        novo->pac = pac;
        novo->prox = NULL;
        fila->fim = novo;
        fila->tamanho++;
        return true;
    }

    return false;
}

PACIENTE* fila_remover(FILA* fila){
    if ((fila != NULL) && (!fila_vazia(fila))){
        NO* atual = fila->inicio;
        PACIENTE* pac = atual->pac;

        fila->inicio = atual->prox;
        atual->prox = NULL;
        free(atual); atual = NULL;

        if (fila->tamanho == 1)
            fila->fim = NULL;

        fila->tamanho--;
        return pac;
    }

    return NULL;
}

bool fila_cheia(FILA* fila){
    if (fila != NULL){
        return (fila->tamanho == TAM_MAX);
    }
    return false;
    
}

bool fila_vazia(FILA* fila){
    if (fila != NULL){
        return (fila->tamanho == 0);
    }
    return true;
}


void fila_apagar(FILA** fila) {
    if (fila == NULL || *fila == NULL) {
        return;
    }

    while (!fila_vazia(*fila)) {
        PACIENTE* pac_removido = fila_remover(*fila);
        paciente_apagar(&pac_removido);
    }

    free(*fila);
    *fila = NULL;
}


PACIENTE *fila_buscar(FILA *fila, char cpf[])
{
  if (fila && !fila_vazia(fila))
  {
    NO* aux = fila->inicio;
    // printf("Lista buscar\n");
    // printf("aux é NULL? %d\n", aux == NULL);
    // printf("aux->pac é NULL? %d\n", aux->pac == NULL);
    while (aux && strcmp(paciente_obter_cpf(aux->pac), cpf) != 0)
    {
      // printf("Prox\n");
      aux = aux->prox;
    }

    return aux ? aux->pac : NULL;
  }

  return NULL;
}

void fila_imprimir(FILA* fila){
    if (fila != NULL){
        if (fila_vazia(fila)){
          printf("A fila está vazia.\n");
          return;
        }
        NO* atual = fila->inicio;
        PACIENTE* pac;
        for (int i = 0; i < fila->tamanho; i++){
            pac = atual->pac;
            paciente_imprimir(pac);
            atual = atual->prox;
        }
    }
}
