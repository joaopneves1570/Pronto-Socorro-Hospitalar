#include "../include/fila.h"
#include "../include/paciente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 50 ///< Capacidade máxima total da fila
#define NUM_PRIORIDADES 5 ///< Quantidade de níveis de prioridade (1 a 5)

typedef struct no_ NO;

/**
 * @brief Nó da fila encadeada.
 * 
 * Cada nó armazena um ponteiro para um paciente e
 * um ponteiro para o próximo nó da fila da mesma prioridade.
 */
struct no_
{
    NO *prox;      ///< Próximo nó na fila
    PACIENTE *pac; ///< Paciente armazenado no nó
};

/**
 * @brief Estrutura da fila de prioridades.
 *
 * Representa 5 filas independentes (uma para cada prioridade),
 * armazenadas em vetores de ponteiros de início e fim.
 */
struct fila_
{
    NO *inicio[NUM_PRIORIDADES]; ///< Vetor de ponteiros para o início de cada fila
    NO *fim[NUM_PRIORIDADES];    ///< Vetor de ponteiros para o fim de cada fila
    int tamanho;                 ///< Quantidade total de pacientes na fila
};

/**
 * @brief Cria uma nova fila de prioridades.
 *
 * @return Ponteiro para FILA alocada e inicializada, ou NULL em caso de erro.
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
 * @brief Insere um paciente na fila conforme sua prioridade.
 *
 * @param fila Ponteiro para a fila.
 * @param pac Paciente a ser inserido.
 * @param prioridade Prioridade do paciente (0 a 4).
 *
 * @return true se inserido com sucesso, false caso contrário.
 *
 * @note A verificação de duplicidade é realizada chamando
 * paciente_esta_na_fila(), conforme requisito do projeto.
 */
bool fila_inserir(FILA *fila, PACIENTE *pac, int prioridade)
{
    if (fila == NULL || fila_cheia(fila)) return false;

    // Verificar duplicidade
    if (paciente_esta_na_fila(pac))
    {
        printf("Paciente já se encontra na fila de espera.\n");
        return false;
    }

    NO *novo = (NO *)malloc(sizeof(NO));
    if (novo == NULL) return false;

    novo->pac = pac;
    novo->prox = NULL;

    // Inserção na fila da prioridade
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
 * @brief Remove o paciente de maior prioridade (menor índice).
 *
 * @param fila Ponteiro para a fila.
 *
 * @return O paciente removido, ou NULL se a fila estiver vazia.
 *
 * @note Entre pacientes da mesma prioridade, vale a ordem de chegada.
 */
PACIENTE *fila_remover(FILA *fila)
{
    if (fila == NULL || fila_vazia(fila)) return NULL;

    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        if (fila->inicio[i] != NULL)
        {
            NO *remover = fila->inicio[i];
            PACIENTE *pac = remover->pac;

            fila->inicio[i] = remover->prox;
            
            if (fila->inicio[i] == NULL)
                fila->fim[i] = NULL;

            free(remover);
            fila->tamanho--;
            paciente_sair_da_fila(pac);

            return pac;
        }
    }
    return NULL;
}

/**
 * @brief Remove o paciente de maior prioridade, informando a prioridade removida.
 *
 * @param fila Ponteiro para a fila.
 * @param prioridade Ponteiro onde será armazenada a prioridade removida (0 a 4).
 *
 * @return Paciente removido, ou NULL caso a fila esteja vazia.
 */
PACIENTE *fila_remover_com_prioridade(FILA *fila, int* prioridade)
{
    if (fila == NULL || fila_vazia(fila)) return NULL;

    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        if (fila->inicio[i] != NULL)
        {
            NO *remover = fila->inicio[i];
            PACIENTE *pac = remover->pac;

            *prioridade = i;

            fila->inicio[i] = remover->prox;

            if (fila->inicio[i] == NULL)
                fila->fim[i] = NULL;

            free(remover);
            fila->tamanho--;
            paciente_sair_da_fila(pac);

            return pac;
        }
    }
    return NULL;
}

/**
 * @brief Verifica se a fila está cheia.
 *
 * @param fila Ponteiro para a fila.
 * @return true se cheia, false caso contrário.
 */
bool fila_cheia(FILA *fila)
{
    if (fila != NULL)
        return (fila->tamanho >= TAM_MAX);
    return true;
}

/**
 * @brief Verifica se a fila está vazia.
 *
 * @param fila Ponteiro para a fila.
 * @return true se vazia, false caso contrário.
 */
bool fila_vazia(FILA *fila)
{
    if (fila != NULL)
        return (fila->tamanho == 0);
    return true;
}

/**
 * @brief Libera toda a memória da fila e seus nós.
 *
 * @param fila Endereço do ponteiro da fila.
 *
 * @warning Após esta chamada, *fila será NULL.
 */
void fila_apagar(FILA **fila)
{
    if (fila == NULL || *fila == NULL) return;

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
 * @brief Imprime todos os pacientes da fila em formato organizado.
 *
 * @param fila Ponteiro para a fila.
 *
 * @note Os pacientes são exibidos por prioridade e na ordem de chegada.
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
