#include "../include/historico.h"

#define TAM_MAX 10

typedef struct no_ NO;

/**
 * @brief Estrutura de um nó da pilha de histórico.
 * * Cada nó armazena a descrição de um procedimento médico e um ponteiro para
 * o nó anterior (abaixo dele na pilha).
 */
struct no_
{
    NO *anterior;
    char procedimento[100];
};

/**
 * @brief Estrutura da pilha de histórico médico.
 * * Implementada como uma pilha (LIFO - Last-In, First-Out) usando uma lista encadeada.
 * Armazena um ponteiro para o topo da pilha e o tamanho atual.
 */
struct historico_
{
    NO *topo;
    int tamanho;
};

/**
 * @brief Aloca e inicializa uma nova estrutura de histórico (pilha).
 * @return Ponteiro para a estrutura HISTORICO alocada ou NULL em caso de erro.
 */
HISTORICO *historico_criar(void)
{
    HISTORICO *hist = (HISTORICO *)malloc(sizeof(HISTORICO));
    if (hist != NULL)
    {
        hist->tamanho = 0;
        hist->topo = NULL;
        return hist;
    }

    return NULL;
}

/**
 * @brief Insere um novo procedimento no topo do histórico (operação push).
 * * Cria um novo nó, copia a string do procedimento para ele e o coloca no topo da pilha.
 * @param hist Ponteiro para o histórico.
 * @param procedimento String contendo a descrição do procedimento a ser adicionado.
 * @return true se a inserção foi bem-sucedida, false caso contrário (histórico cheio ou erro de alocação).
 */
bool historico_inserir(HISTORICO *hist, char procedimento[])
{
    if (hist != NULL && !historico_cheio(hist))
    {
        NO *novo = (NO *)malloc(sizeof(NO));
        if (novo != NULL)
        {
            snprintf(novo->procedimento, 100, "%s", procedimento);
            novo->anterior = hist->topo;
            hist->topo = novo;
            hist->tamanho++;

            return true;
        }
    }
    return false;
}

/**
 * @brief Remove e retorna o procedimento mais recente do topo do histórico (operação pop).
 * @details Esta função aloca dinamicamente uma nova string para o procedimento removido.
 * O chamador é responsável por liberar essa memória com free().
 * @param hist Ponteiro para o histórico.
 * @return Ponteiro para uma nova string com o procedimento removido, ou NULL se o histórico estiver vazio.
 */
char *historico_remover(HISTORICO *hist)
{
    if (hist != NULL && !historico_vazio(hist))
    {
        NO *aux = hist->topo;

        char *procedimento = calloc(100, sizeof(char));
        if (procedimento == NULL) return NULL; // Verificação de segurança

        strcpy(procedimento, hist->topo->procedimento);

        hist->topo = aux->anterior;
        aux->anterior = NULL;
        free(aux);
        aux = NULL;

        hist->tamanho--;

        return procedimento;
    }
    return NULL;
}

/**
 * @brief Consulta o procedimento no topo do histórico sem removê-lo (operação top/peek).
 * @details O procedimento é copiado para o buffer de char fornecido pelo chamador.
 * @param hist Ponteiro para o histórico.
 * @param procedimento Ponteiro para um buffer de char onde o procedimento será copiado.
 * @return true se a consulta foi bem-sucedida, false se o histórico estiver vazio.
 */
bool historico_consultar(HISTORICO *hist, char *procedimento)
{
    if (hist != NULL && !historico_vazio(hist))
    {

        strcpy(procedimento, hist->topo->procedimento);

        return true;
    }
    return false;
}

/**
 * @brief Verifica se o histórico atingiu sua capacidade máxima.
 * @param hist Ponteiro para o histórico.
 * @return true se o histórico está cheio, false caso contrário.
 */
bool historico_cheio(HISTORICO *hist)
{
    if (hist != NULL)
    {
        return hist->tamanho == TAM_MAX;
    }
    return false;
}

/**
 * @brief Verifica se o histórico não contém nenhum procedimento.
 * @param hist Ponteiro para o histórico.
 * @return true se o histórico está vazio, false caso contrário.
 */
bool historico_vazio(HISTORICO *hist)
{
    if (hist != NULL)
    {
        return hist->tamanho == 0;
    }
    return false;
}

/**
 * @brief Retorna o número de procedimentos atualmente no histórico.
 * @param hist Ponteiro para o histórico.
 * @return O número de procedimentos no histórico, ou 0 (false) se o histórico for nulo.
 */
int historico_tamanho(HISTORICO *hist)
{
    if (hist != NULL)
    {
        return hist->tamanho;
    }
    return false;
}

/**
 * @brief Libera toda a memória associada a um histórico.
 * @details Desaloca a memória de todos os nós da pilha e da própria estrutura de histórico.
 * O ponteiro original que aponta para o histórico é definido como NULL.
 * @param hist Ponteiro para o ponteiro do histórico a ser apagado.
 */
void historico_apagar(HISTORICO **hist)
{
    if (hist != NULL && *hist != NULL)
    {
        while ((*hist)->topo != NULL)
        {
            NO *aux = (*hist)->topo;
            (*hist)->topo = aux->anterior;
            aux->anterior = NULL;
            free(aux);
            aux = NULL;
        }
        free(*hist);
        *hist = NULL;
    }
}

/**
 * @brief Imprime todos os procedimentos do histórico, do mais recente para o mais antigo.
 * @param hist Ponteiro para o histórico a ser impresso.
 */
void historico_imprimir(HISTORICO *hist)
{
    if (hist != NULL)
    {
        if (historico_vazio(hist))
        {
            printf("O histórico do paciente está vazio.\n");
            return;
        }
        printf("--- Histórico de Procedimentos ---\n");
        for (NO *no = hist->topo; no != NULL; no = no->anterior)
        {
            printf("- %s\n", no->procedimento);
        }
        printf("----------------------------------\n");
    }
}