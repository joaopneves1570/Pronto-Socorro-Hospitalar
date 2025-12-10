/**
 * @file historico.c
 * @brief Implementação de um histórico de ações (Log).
 * @details Esta estrutura funciona como uma Pilha (Stack) de capacidade fixa (10 itens).
 * Armazena strings de até 100 caracteres. Quando cheia, não aceita novos itens até que
 * algum seja removido.
 */

#include "../include/historico.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @struct historico_
 * @brief Estrutura opaca que representa o histórico.
 */
struct historico_{
    char hist[10][100]; /**< Matriz estática armazenando até 10 strings de 100 chars. */
    int tamanho;        /**< Contador de quantos elementos estão atualmente no histórico. */
};

/**
 * @brief Cria uma nova instância de histórico.
 * @return HISTORICO* Ponteiro para a estrutura alocada ou NULL se falhar.
 */
HISTORICO* historico_criar(void){
    HISTORICO* hist = (HISTORICO*)malloc(sizeof(HISTORICO));
    if (hist != NULL){
        hist->tamanho = 0;
    }

    return hist;
}

/**
 * @brief Retorna a quantidade de itens armazenados no histórico.
 * @param hist Ponteiro para o histórico.
 * @return int O tamanho atual ou -1 se o ponteiro for NULL.
 */
int historico_tamanho(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho;
    }

    return -1;
}

/**
 * @brief Verifica se o histórico atingiu sua capacidade máxima (10 itens).
 * @param hist Ponteiro para o histórico.
 * @return true Se estiver cheio.
 * @return false Caso contrário ou se ponteiro for NULL.
 */
bool historico_cheio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == 10;
    }

    return false;
}

/**
 * @brief Verifica se o histórico está vazio.
 * @param hist Ponteiro para o histórico.
 * @return true Se tamanho for 0 ou ponteiro for NULL.
 * @return false Se houver itens.
 */
bool historico_vazio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == 0;
    }

    return true;
}

/**
 * @brief Adiciona uma nova string ao topo do histórico.
 * @details Realiza uma cópia da string passada para a matriz interna.
 * @param hist Ponteiro para o histórico.
 * @param texto String a ser armazenada (deve ter no máx 99 chars + \0).
 * @return true Se inserido com sucesso.
 * @return false Se o histórico estiver cheio ou inválido.
 */
bool historico_inserir(HISTORICO* hist, char texto[]){
    if (hist != NULL && !historico_cheio(hist)){
        strcpy(hist->hist[hist->tamanho], texto);
        hist->tamanho++;
        return true;
    }

    return false;
}

/**
 * @brief Remove a última entrada adicionada ao histórico (comportamento LIFO).
 * @param hist Ponteiro para o histórico.
 * @return char* Ponteiro para a string que acabou de ser "removida" (ainda reside na memória interna).
 * @return NULL Se o histórico estiver vazio.
 */
char* historico_remover(HISTORICO* hist){
    if (hist != NULL && !historico_vazio(hist)){
        hist->tamanho--;
        return hist->hist[hist->tamanho];
    }

    return NULL;
}

/**
 * @brief Verifica se uma determinada string existe no histórico.
 * @details Realiza uma busca linear.
 * @param hist Ponteiro para o histórico.
 * @param texto String a ser buscada.
 * @return true Se a string foi encontrada.
 * @return false Se não encontrada.
 */
bool historico_consultar(HISTORICO* hist, char* texto){
    if (hist != NULL){
        for (int i = 0; i < hist->tamanho; i++){
            if (strcmp(hist->hist[i], texto) == 0){
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Imprime todo o conteúdo do histórico no terminal.
 * @param hist Ponteiro para o histórico.
 */
void historico_imprimir(HISTORICO* hist){
    if (hist != NULL){
        for (int i = hist->tamanho; i >= 0; i--){
            printf("%s\n", hist->hist[i]);
        }
    }
}

/**
 * @brief Libera a memória alocada para o histórico.
 * @param hist Endereço do ponteiro do histórico (HISTORICO**).
 */
void historico_apagar(HISTORICO** hist){
    if (hist != NULL && *hist != NULL){
        free(*hist);
        *hist = NULL;
    }
}