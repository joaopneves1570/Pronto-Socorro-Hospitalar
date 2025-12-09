#include "../include/fila.h"
#include "../include/paciente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 50 // Capacidade total da fila (soma de todas as prioridades)
#define NUM_PRIORIDADES 5

typedef struct no_ NO;

/**
 * @brief Estrutura de um nó da fila.
 */
struct no_
{
    NO *prox;
    PACIENTE *pac;
};

/**
 * @brief Estrutura da fila de prioridades.
 * Utiliza um vetor de ponteiros para gerenciar 5 filas independentes,
 * uma para cada nível de prioridade.
 */
struct fila_
{
    NO *inicio[NUM_PRIORIDADES]; // Vetor de inícios (Prioridade 1 no índice 0)
    NO *fim[NUM_PRIORIDADES];    // Vetor de fins
    int tamanho;                 // Quantidade total de pacientes na estrutura
};

/**
 * @brief Aloca e inicializa as 5 filas de prioridade.
 */
FILA *fila_criar()
{
    FILA *fila = (FILA *)malloc(sizeof(FILA));
    if (fila != NULL)
    {
        for (int i = 0; i < NUM_PRIORIDADES; i++)
        {
            fila->inicio[i] = NULL;
            fila->fim[i] = NULL;
        }
        fila->tamanho = 0;
    }
    return fila;
}


/**
 * @brief Insere um paciente na fila de acordo com sua prioridade.
 * O(1) para inserção. A busca de duplicidade depende da implementação,
 * aqui é feita linearmente para garantir integridade, mas a inserção na estrutura é direta.
 */
bool fila_inserir(FILA *fila, PACIENTE *pac, int prioridade)
{
    if (fila == NULL || fila_cheia(fila)) return false;

    // Verifica se o paciente já está na fila (Requisito do projeto)
    if (paciente_esta_na_fila(pac))
    {
        printf("Paciente já se encontra na fila de espera.\n");
        return false;
    }

    NO *novo = (NO *)malloc(sizeof(NO));
    if (novo == NULL) return false;

    novo->pac = pac;
    novo->prox = NULL;

    // Inserção na fila específica da prioridade (Bucket)
    if (fila->inicio[prioridade] == NULL)
    {
        fila->inicio[prioridade] = novo;
    }
    else
    {
        fila->fim[prioridade]->prox = novo;
    }

    fila->fim[prioridade] = novo;
    fila->tamanho++;

    paciente_ir_para_fila(pac);
    
    return true;
}

/**
 * @brief Remove o paciente com a maior prioridade (menor índice).
 * Respeita a ordem de chegada dentro da mesma prioridade.
 */
PACIENTE *fila_remover(FILA *fila)
{
    if (fila == NULL || fila_vazia(fila)) return NULL;

    // Procura na ordem de prioridade (0=Emergência até 4=Não Urgente)
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        if (fila->inicio[i] != NULL)
        {
            NO *remover = fila->inicio[i];
            PACIENTE *pac = remover->pac;

            fila->inicio[i] = remover->prox;
            
            if (fila->inicio[i] == NULL)
            {
                fila->fim[i] = NULL;
            }

            free(remover);
            fila->tamanho--;
            paciente_sair_da_fila(pac);
            return pac;
        }
    }

    return NULL;
}

PACIENTE *fila_remover_com_prioridade(FILA *fila, int* prioridade)
{
    if (fila == NULL || fila_vazia(fila)) return NULL;

    // Procura na ordem de prioridade (0=Emergência até 4=Não Urgente)
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        if (fila->inicio[i] != NULL)
        {
            NO *remover = fila->inicio[i];
            PACIENTE *pac = remover->pac;

            *prioridade = i;

            fila->inicio[i] = remover->prox;
            
            if (fila->inicio[i] == NULL)
            {
                fila->fim[i] = NULL;
            }

            free(remover);
            fila->tamanho--;
            paciente_sair_da_fila(pac);
            return pac;
        }
    }

    return NULL;
}

bool fila_cheia(FILA *fila)
{
    if (fila != NULL)
        return (fila->tamanho >= TAM_MAX);
    return true;
}

bool fila_vazia(FILA *fila)
{
    if (fila != NULL)
        return (fila->tamanho == 0);
    return true;
}

void fila_apagar(FILA **fila)
{
    if (fila == NULL || *fila == NULL) return;

    // Percorre todas as prioridades limpando os nós
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        NO *atual = (*fila)->inicio[i];
        while (atual != NULL)
        {
            NO *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }

    free(*fila);
    *fila = NULL;
}


/**
 * @brief Imprime a fila completa, ordenada por prioridade.
 */
void fila_imprimir(FILA *fila)
{
    if (fila == NULL || fila_vazia(fila))
    {
        printf("A fila de espera está vazia.\n");
        return;
    }

    const char *descricoes[5] = {
        "Emergência", "Muito Urgente", "Urgente", "Pouco Urgente", "Não Urgência"
    };

    printf("\n=== FILA DE ESPERA (Por Prioridade) ===\n");
    int posicao_global = 1;

    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        if (fila->inicio[i] != NULL)
        {
            printf("\n--- Prioridade %d: %s ---\n", i + 1, descricoes[i]);
            NO *atual = fila->inicio[i];
            while (atual != NULL)
            {
                printf("%dº Geral | ", posicao_global++);
                paciente_imprimir(atual->pac);
                atual = atual->prox;
            }
        }
    }
    printf("=======================================\n");
}