#include "../include/lista.h"
#include <string.h> // Necessário para strcmp
#include <stdio.h>
#include <stdlib.h>

typedef struct no_ NO;
struct no_{
    NO* esq;
    NO* dir;
    PACIENTE* pac;
    int altura;
};

struct lista_{
    NO* raiz;
};

typedef void (*AcaoPaciente)(PACIENTE* p, void* contexto);

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

// --- Rotações ---

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

// --- Inserção ---

NO* lista_inserir_no(NO* raiz, PACIENTE* p){
    if (raiz == NULL)
        raiz = lista_cria_no(p);
    
    // CORREÇÃO: Usando strcmp em vez de atol
    int cmp = strcmp(paciente_obter_cpf(p), paciente_obter_cpf(raiz->pac));

    if (cmp < 0){
        raiz->esq = lista_inserir_no(raiz->esq, p);
    } else if (cmp > 0){
        raiz->dir = lista_inserir_no(raiz->dir, p);
    }
    // Se cmp == 0, CPF é igual, não insere duplicado (ou atualiza, dependendo da lógica desejada)

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
        // Verifica se a raiz mudou (pode ter rotacionado ou sido criada)
        l->raiz = lista_inserir_no(l->raiz, p);
        return (l->raiz != NULL); 
    }
    return false;
}

// --- Remoção ---

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

// CORREÇÃO: Recebe char* chave em vez de long int
NO* lista_remover_no(NO* raiz, char* chave, PACIENTE** pac){
    NO* p;
    
    if (raiz == NULL) return NULL;

    // CORREÇÃO: Usando strcmp
    int cmp = strcmp(chave, paciente_obter_cpf(raiz->pac));

    if (cmp == 0){ // Encontrou
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
    } else if (cmp < 0){
        raiz->esq = lista_remover_no(raiz->esq, chave, pac);
    } else if (cmp > 0){
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
        // Passa a string CPF direto
        l->raiz = lista_remover_no(l->raiz, paciente_obter_cpf(p), &paciente_recuperado);
        return paciente_recuperado;
    }
    return NULL;
}

PACIENTE* lista_remover_ultimo(LISTA* l){
    if (l != NULL && !(lista_vazia(l))){
        PACIENTE* paciente_recuperado = NULL;
        NO* atual = l->raiz;
        
        // CORREÇÃO: O loop deve verificar se 'atual' tem direita
        while (atual->dir != NULL){
            atual = atual->dir;
        }
        
        // CORREÇÃO: Captura o CPF antes de remover
        // IMPORTANTE: Devemos chamar remover_no a partir da RAIZ (l->raiz) para rebalancear a árvore inteira,
        // e não a partir de 'atual' (que quebraria a árvore).
        char cpf_ultimo[16]; // Buffer seguro para guardar o CPF
        strcpy(cpf_ultimo, paciente_obter_cpf(atual->pac));

        l->raiz = lista_remover_no(l->raiz, cpf_ultimo, &paciente_recuperado);
        
        return paciente_recuperado;
    }
    return NULL;
}

// --- Utilidades ---

bool lista_vazia(LISTA* l){
    if (l != NULL){
        return l->raiz == NULL;
    }
    return true;
}

bool lista_cheia(LISTA* l){
    return false; // Implementação dinâmica, nunca cheia (a menos que acabe RAM)
}

// CORREÇÃO: Recebe char* cpf em vez de long int
NO* lista_buscar_no(NO* raiz, char* cpf, PACIENTE** p){
    if (raiz == NULL)
        return NULL;
    
    // CORREÇÃO: strcmp
    int cmp = strcmp(cpf, paciente_obter_cpf(raiz->pac));
    
    if (cmp == 0){
        *p = raiz->pac;
        return raiz;
    }
    else if (cmp < 0)
        return lista_buscar_no(raiz->esq, cpf, p);
    else
        return lista_buscar_no(raiz->dir, cpf, p);
}

PACIENTE* lista_buscar(LISTA* l, char* cpf){
    if (l != NULL){
        PACIENTE* paciente_buscado = NULL;
        // Passa a string CPF direto
        lista_buscar_no(l->raiz, cpf, &paciente_buscado);
        return paciente_buscado;
    }
    return NULL;
}

// --- Impressão e Limpeza ---

void lista_em_ordem(NO* raiz, AcaoPaciente acao, void* contexto){
    if (raiz != NULL){
        lista_em_ordem(raiz->esq, acao, contexto);
        acao(raiz->pac, contexto);
        lista_em_ordem(raiz->dir, acao, contexto);
    }
}

void lista_pre_ordem(NO* raiz, AcaoPaciente acao, void* contexto){
    if (raiz != NULL){
        acao(raiz->pac, contexto); 
        lista_pre_ordem(raiz->esq, acao, contexto); 
        lista_pre_ordem(raiz->dir, acao, contexto);
    }
}

void acao_imprimir_lista(PACIENTE* p, void* contexto){
    char* estado;
    if (paciente_esta_na_fila(p))
        estado = "NA FILA DE ATENDIMENTO";
    else
        estado = "FORA DA FILA DE ATENDIMENTO";
    
    printf("NOME: %s\nCPF: %s\nESTADO: %s\n\n", paciente_obter_nome(p), paciente_obter_cpf(p), estado);  
}

void lista_mostrar(LISTA* l){
    if (l != NULL){
        printf("Lista de Pacientes (em ordem crescente de CPF):\n");
        lista_em_ordem(l->raiz, acao_imprimir_lista, NULL);    
    }
}

void lista_apagar_aux(NO* raiz){
    if (raiz != NULL){
        lista_apagar_aux(raiz->esq);
        lista_apagar_aux(raiz->dir);
        // Nota: O TAD Lista é "dono" do paciente? Se sim, apague.
        paciente_apagar(&raiz->pac); 
        free(raiz);
    }
}

void lista_apagar(LISTA** l){
    if (*l != NULL){
        lista_apagar_aux((*l)->raiz);
        free(*l);
        *l = NULL;
    }
}

