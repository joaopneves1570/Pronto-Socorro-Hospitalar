#include "../include/lista.h"
#include <stdio.h>
#include <string.h>

typedef struct no_ NO;

struct no_{
    PACIENTE* pac;
    NO* prox;
    NO* ant;
};

struct lista_ {
    NO* inicio;
    int tamanho;
};

LISTA* lista_criar(){
    LISTA* lista = (LISTA*)malloc(sizeof(LISTA));
    if (lista != NULL){
        NO* cabeca = (NO*)malloc(sizeof(NO));
        if (cabeca != NULL){
            cabeca->pac = paciente_criar("CABEÇUDO", "00000000000");
            lista->inicio = cabeca;
            lista->inicio->prox = lista->inicio;
            lista->inicio->ant = lista->inicio;
            lista->tamanho = 0;
            return lista;
        } 
    }

    return NULL;
}

bool lista_inserir(LISTA* lista, PACIENTE* p){
    if (lista != NULL && !lista_cheia(lista)){
        NO* novo = (NO*)malloc(sizeof(NO));
        if (novo != NULL){
            novo->pac = p;
            novo->prox = lista->inicio;
            novo->ant = novo->prox->ant;
            novo->ant->prox = novo;
            novo->prox->ant = novo;
            lista->tamanho++;
            return true;
        }
    }
    return false;
}

PACIENTE* lista_remover(LISTA* lista, PACIENTE* pac){
  printf("lista_remover\n");
    if ((lista != NULL) && (pac != NULL) && (!lista_vazia(lista))){
        paciente_definir_cpf(lista->inicio->pac, paciente_obter_cpf(pac));
        NO* aux = lista->inicio->prox;
        char* cpf_paciente = paciente_obter_cpf(pac);
        // Enquanto o cpf do paciente atual for diferente do esperado vai pro próximo
        while (strcmp(paciente_obter_cpf(aux->pac), cpf_paciente)) aux = aux->prox;

        printf("Parou a busca.\n");
        if (aux != lista->inicio){
            aux->ant->prox = aux->prox;
            aux->prox->ant = aux->ant;
            aux->ant = NULL;
            aux->prox = NULL;

            PACIENTE* paciente = aux->pac;

            free(aux);

            return paciente;
        }
    }

    return NULL;
}

PACIENTE* lista_remover_inicio(LISTA* lista){
    if ((lista != NULL) && (!lista_vazia(lista))){
        NO* no = lista->inicio->prox;
        
        PACIENTE* paciente = no->pac;

        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
        no->ant = NULL;
        no->prox = NULL;

        free(no);

        return paciente;
    }

    return NULL;
}

bool lista_vazia(LISTA* l){
    if (l != NULL)
        return l->inicio->prox == l->inicio;
    return false;
}

bool lista_cheia(LISTA* l){
    if (l != NULL){
        NO* teste = (NO*)malloc(sizeof(NO));
        if (teste != NULL){
            free(teste);
            teste = NULL;
            return false;
        } else {
            return true;
        }
    }
    return false;
}

PACIENTE* lista_buscar(LISTA* l, char* cpf){
    if ((l != NULL) && (!lista_vazia(l))){
        paciente_definir_cpf(l->inicio->pac, cpf);
        NO* aux = l->inicio->prox;
        while ((strcmp(paciente_obter_cpf(aux->pac), cpf) != 0)){
            aux = aux->prox;
        }

        if (aux != l->inicio)
            return aux->pac;
    }

    return NULL;
}

void lista_mostrar(LISTA* l) {
    if ((l != NULL) && (!lista_vazia(l))){
        NO* aux = l->inicio->prox;
        while (aux != l->inicio){
            paciente_imprimir(aux->pac);
            aux = aux->prox;
        }
    }
}


void lista_apagar(LISTA** l) {
    if (l == NULL || *l == NULL) {
        return;
    }

    NO* no_atual = (*l)->inicio->prox;
    NO* no_proximo;
    
    while (no_atual != (*l)->inicio) {
        no_proximo = no_atual->prox;
        paciente_apagar(&(no_atual->pac));        
        free(no_atual);
        no_atual = no_proximo;
    }
    free((*l)->inicio);

   
    free(*l);
    *l = NULL;
}
