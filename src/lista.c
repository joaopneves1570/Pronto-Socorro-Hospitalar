#include "../include/lista.h"

typedef struct no_ NO;
struct no_{
    NO* esq;
    NO* dir;
    PACIENTE* pac;
    int altura;
};

struct avl_{
    NO* raiz;
};

LISTA* lista_criar(void){
    LISTA* lista = (LISTA*)malloc(sizeof(LISTA));
    if (lista != NULL){
        lista->raiz = NULL;
    }

    return lista;
}

NO* lista_cria_no(PACIENTE* p){
    NO* novo = (NO*)malloc(sizeof(NO));
    if (novo != NULL){
        novo->altura = 0;
        novo->dir = NULL;
        novo->esq = NULL;
        novo->pac = p;
    }

    return novo;
}

int lista_altura_no(NO* raiz){
    if (raiz != NULL){
        return raiz->altura;
    }

    return -1;
}

NO* rodar_direita(NO* a){
    NO* b = a->esq;
    a->esq = b->dir;
    b->dir = a;

    a->altura = max(lista_altura_no(a->esq), lista_altura_no(a->dir)) + 1;
    b->altura = max(lista_altura_no(b->esq), lista_altura_no(b->dir)) + 1;

    return b;
}

NO* rodar_esquerda(NO* a){
    NO* b = a->dir;
    a->dir = b->esq;
    b->esq = a;

    a->altura = max(lista_altura_no(a->esq), lista_altura_no(a->dir)) + 1;
    b->altura = max(lista_altura_no(b->esq), lista_altura_no(b->dir)) + 1;

    return b;
}

NO* rodar_direita_esquerda(NO* a){
    NO* b = a->dir;
    b = rodar_direita(b);
    return rodar_esquerda(a);
}

NO* rodar_esquerda_direita(NO* a){
    NO* b = a->esq;
    b = rodar_esquerda(b);
    return rodar_direita(a);
}

NO* lista_inserir_no(NO* raiz, PACIENTE* p){
    if (raiz == NULL)
        raiz = lista_cria_no(p);
    
    long int chave_item = atol(paciente_obter_cpf(p));
    long int chave_raiz = atol(paciente_obter_cpf(raiz->pac));

    if (chave_item < chave_raiz){
        raiz->esq = lista_inserir_no(raiz->esq, p);
    } else if (chave_item > chave_raiz){
        raiz->dir = lista_inserir_no(raiz->dir, p);
    }

    raiz->altura = max(lista_altura_no(raiz->esq), lista_altura_no(raiz->dir)) + 1;
    int FB = lista_altura_no(raiz->esq) - lista_altura_no(raiz->dir);

    if (FB == -2){
        if (lista_altura_no(raiz->dir->esq) - lista_altura_no(raiz->dir->dir) <= 0)
            raiz = rodar_esquerda(raiz);
        else
            raiz = rodar_direita_esquerda(raiz);
    } else if (FB == 2){
        if (lista_altura_no(raiz->esq->esq) - lista_altura_no(raiz->esq->dir) >= 0)
            raiz = rodar_direita(raiz);
        else
            raiz = rodar_esquerda_direita(raiz);
    }

    return raiz;
}

bool lista_inserir(LISTA* l, PACIENTE* p){
    if (l != NULL){
        return ((l->raiz = lista_inserir_no(l->raiz, p)) != NULL);
    }

    return false;
}

void troca_max_esq(NO* atual, NO* raiz, NO* ant, PACIENTE** pac){

    if (atual->dir != NULL){
        troca_max_esq(atual->dir, raiz, atual, pac);
        return;
    }

    if (ant == raiz)
        ant->esq = atual->esq;
    else
        ant->dir = atual->esq;
    
    if (pac != NULL)
        *pac = raiz->pac;

    raiz->pac = atual->pac;

    free(atual);
    atual = NULL;
    
}

NO* lista_remover_no(NO* raiz, long int chave, PACIENTE** pac){
    NO* p;
    long int chave_raiz = atol(paciente_obter_cpf(raiz->pac));

    if (raiz == NULL)
        return NULL;
    if (chave == chave_raiz){
        if (raiz->esq == NULL || raiz->dir == NULL){
            p = raiz;

            if (pac != NULL) {
                *pac = p->pac;
            }

            if (raiz->esq == NULL)
                raiz = raiz->dir;
            else
                raiz = raiz->esq;

            
            free(p);
            p = NULL;

        } else {
            troca_max_esq(raiz->esq, raiz, raiz, pac);
        }
    } else if (chave < chave_raiz ){
        raiz->esq = lista_remover_no(raiz->esq, chave, pac);
    } else if (chave > chave_raiz){
        raiz->dir = lista_remover_no(raiz->dir, chave, pac);
    }

    if (raiz != NULL){
        raiz->altura = max(lista_altura_no(raiz->esq), lista_altura_no(raiz->dir)) + 1;

        int FB = lista_altura_no(raiz->esq) - lista_altura_no(raiz->dir);

        if (FB == -2){
            if (lista_altura_no(raiz->dir->esq) - lista_altura_no(raiz->dir->dir) <= 0)
                raiz = rodar_esquerda(raiz);
            else
                raiz = rodar_direita_esquerda(raiz);
        } else if (FB == 2){
            if (lista_altura_no(raiz->esq->esq) - lista_altura_no(raiz->esq->dir) >= 0)
                raiz = rodar_direita(raiz);
            else
                raiz = rodar_esquerda_direita(raiz);
        }
        
    }

    return raiz;
}

PACIENTE* lista_remover(LISTA* l, PACIENTE* p){
    if (l != NULL && !(lista_vazia(l))){
        PACIENTE* paciente_recuperado = NULL;
        l->raiz = lista_remover_no(l->raiz, atol(paciente_obter_cpf(p)), &paciente_recuperado);
        return paciente_recuperado;
    }

    return NULL;
}

PACIENTE* lista_remover_inicio(LISTA* l){
    if (l != NULL && !(lista_vazia(l))){
        PACIENTE* paciente_recuperado = NULL;
        l->raiz = lista_remover_no(l->raiz, atol(paciente_obter_cpf(l->raiz->pac)), &paciente_recuperado);
        return paciente_recuperado;
    }

    return NULL;
}

bool lista_vazia(LISTA* l){
    if (l != NULL){
        return l->raiz == NULL;
    }

    return true;
}

bool lista_cheia(LISTA* l){
    if (l != NULL){
        NO* teste = (NO*)malloc(sizeof(NO));
        if (teste == NULL){
            return true;
        }
        free(teste);
        teste = NULL;
    }
    return false;
}

NO* lista_buscar_no(NO* raiz, long int cpf, PACIENTE** p){
    if (raiz == NULL)
        return NULL;
    
    long int cpf_raiz = atol(paciente_obter_cpf(raiz->pac));
    
    if (cpf == cpf_raiz){
        *p = raiz->pac;
        return raiz;
    }
    else if (cpf < cpf_raiz)
        return lista_buscar_no(raiz->esq, cpf, p);
    else if (cpf > cpf_raiz)
        return lista_buscar_no(raiz->dir, cpf, p);
    
}

PACIENTE* lista_buscar(LISTA* l, char* cpf){
    if (l != NULL){
        PACIENTE* paciente_buscado = NULL;
        l->raiz = lista_buscar_no(l->raiz, atol(cpf), &paciente_buscado);
        return paciente_buscado;
    }

    return NULL;
}

void lista_em_ordem(NO* raiz){
    if (raiz != NULL){
        lista_em_ordem(raiz->esq);
        printf("Nome do paciente: %s\n", paciente_obter_nome(raiz->pac));
        printf("CPF do paciente: %s\n", paciente_obter_cpf(raiz->pac));
        lista_em_ordem(raiz->dir);
    }
}

void lista_mostrar(LISTA* l){
    if (l != NULL){
        lista_em_ordem(l->raiz);
    }
}

void lista_apagar_aux(NO* raiz){
    if (raiz != NULL){
        lista_apagar_aux(raiz->esq);
        lista_apagar_aux(raiz->dir);
        paciente_apagar(&raiz->pac);
        free(raiz);
        raiz = NULL;
    }
}

void lista_apagar(LISTA** l){
    if (*l != NULL){
        lista_apagar_aux((*l)->raiz);
        free(*l);
        *l = NULL;
    }
}


