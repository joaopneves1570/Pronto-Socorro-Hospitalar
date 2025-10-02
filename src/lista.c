#include "lista.h"

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

bool lista_remover(LISTA* lista, PACIENTE* pac){
    if ((lista != NULL) && (!lista_vazia(lista))){
        NO* aux = lista->inicio->prox;
        while (aux != lista->inicio){
            if (aux->pac == pac) break;
            aux = aux->prox;
        }

        if (aux != lista->inicio){
            paciente_registrar_obito(&(aux->pac));
            aux->ant->prox = aux->prox;
            aux->prox->ant = aux->ant;
            aux->ant = NULL;
            aux->prox = NULL;
            free(aux); aux = NULL;
            lista->tamanho--;
            return true;
        }
    }

    return false;
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

PACIENTE* lista_buscar(LISTA* l, int id){
    if ((l != NULL) && (!lista_vazia(l))){
        NO* aux = l->inicio->prox;
        while ((paciente_get_id(aux->pac) != id) && (aux != l->inicio)){
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
        paciente_registrar_obito(&(no_atual->pac));        
        free(no_atual);
        no_atual = no_proximo;
    }
    free((*l)->inicio);

   
    free(*l);
    *l = NULL;
}