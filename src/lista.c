#include "../include/lista.h"
#include "../include/paciente.h" 

typedef struct no_ NO;

/**
 * @brief Estrutura de um nó da lista duplamente encadeada.
 * * Cada nó contém um ponteiro para um paciente e ponteiros para o
 * nó próximo e o nó anterior na lista.
 */
struct no_
{
    PACIENTE *pac;
    NO *prox;
    NO *ant;
};

/**
 * @brief Estrutura da lista de pacientes.
 * * Implementada como uma lista duplamente encadeada circular com um nó cabeça (sentinela).
 * O nó cabeça simplifica as operações, evitando casos de borda.
 */
struct lista_
{
    NO *inicio; // Ponteiro para o nó cabeça (sentinela)
    int tamanho;
};

/**
 * @brief Aloca e inicializa uma nova lista vazia com um nó cabeça.
 * @details O nó cabeça (sentinela) é um nó especial que não armazena dados de pacientes
 * e cujos ponteiros 'prox' e 'ant' apontam para ele mesmo em uma lista vazia.
 * @return Ponteiro para a estrutura LISTA alocada ou NULL em caso de erro.
 */
LISTA *lista_criar()
{
    LISTA *lista = (LISTA *)malloc(sizeof(LISTA));
    if (lista != NULL)
    {
        NO *cabeca = (NO *)malloc(sizeof(NO));
        if (cabeca != NULL)
        {
            cabeca->pac = paciente_criar("CABECA", "00000000000"); // Paciente sentinela
            lista->inicio = cabeca;
            lista->inicio->prox = lista->inicio;
            lista->inicio->ant = lista->inicio;
            lista->tamanho = 0;
            return lista;
        }
        free(lista); // Libera a lista se a alocação do nó cabeça falhar
    }

    return NULL;
}

/**
 * @brief Insere um novo paciente na lista.
 * @details A inserção é sempre feita após o último elemento (antes do nó cabeça),
 * aproveitando a estrutura circular da lista.
 * @param lista Ponteiro para a lista.
 * @param p Ponteiro para o paciente a ser inserido.
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool lista_inserir(LISTA *lista, PACIENTE *p)
{
    if (lista != NULL && !lista_cheia(lista))
    {
        NO *novo = (NO *)malloc(sizeof(NO));
        if (novo != NULL)
        {
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

/**
 * @brief Remove um paciente específico da lista, buscado pelo CPF.
 * @details Utiliza o nó cabeça como sentinela para a busca, colocando o CPF do paciente
 * a ser removido no paciente do nó cabeça. Isso garante que a busca sempre encontrará o
 * CPF, simplificando o loop ao remover a verificação de final de lista.
 * @param lista Ponteiro para a lista.
 * @param pac Ponteiro para o paciente a ser removido (usado para obter o CPF).
 * @return Ponteiro para o PACIENTE removido, ou NULL se não for encontrado.
 */
PACIENTE *lista_remover(LISTA *lista, PACIENTE *pac)
{
    if ((lista != NULL) && (pac != NULL) && (!lista_vazia(lista)))
    {
        paciente_definir_cpf(lista->inicio->pac, paciente_obter_cpf(pac));
        NO *aux = lista->inicio->prox;

        // O loop para quando encontra o CPF (garantido pelo sentinela)
        while (strcmp(paciente_obter_cpf(aux->pac), paciente_obter_cpf(pac)) != 0)
        {
            aux = aux->prox;
        }

        // Se o loop parou antes do nó cabeça, o paciente foi encontrado
        if (aux != lista->inicio)
        {
            aux->ant->prox = aux->prox;
            aux->prox->ant = aux->ant;
            PACIENTE *paciente_removido = aux->pac;
            free(aux);
            lista->tamanho--;
            return paciente_removido;
        }
    }

    return NULL;
}

/**
 * @brief Remove o primeiro paciente da lista.
 * @param lista Ponteiro para a lista.
 * @return Ponteiro para o PACIENTE removido ou NULL se a lista estiver vazia.
 */
PACIENTE *lista_remover_inicio(LISTA *lista)
{
    if ((lista != NULL) && (!lista_vazia(lista)))
    {
        NO *no = lista->inicio->prox;

        PACIENTE *paciente = no->pac;

        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
        no->ant = NULL;
        no->prox = NULL;

        free(no);
        lista->tamanho--;
        return paciente;
    }

    return NULL;
}

/**
 * @brief Verifica se a lista está vazia.
 * @details Uma lista com nó cabeça está vazia se o ponteiro 'prox' do nó cabeça aponta para ele mesmo.
 * @param l Ponteiro para a lista.
 * @return true se a lista está vazia, false caso contrário.
 */
bool lista_vazia(LISTA *l)
{
    if (l != NULL)
        return l->inicio->prox == l->inicio;
    return false;
}

/**
 * @brief Verifica se a memória está cheia, tentando alocar um novo nó.
 * @param l Ponteiro para a lista.
 * @return true se não há memória disponível, false caso contrário.
 */
bool lista_cheia(LISTA *l)
{
    if (l != NULL)
    {
        NO *teste = (NO *)malloc(sizeof(NO));
        if (teste != NULL)
        {
            free(teste);
            teste = NULL;
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Busca um paciente na lista pelo seu CPF.
 * @details Também utiliza o nó cabeça como sentinela para otimizar a busca. O CPF
 * é temporariamente copiado para o paciente do nó cabeça, garantindo que o loop de
 * busca sempre termine.
 * @param l Ponteiro para a lista.
 * @param cpf String com o CPF a ser buscado.
 * @return Ponteiro para o PACIENTE encontrado ou NULL se não estiver na lista.
 */
PACIENTE *lista_buscar(LISTA *l, char *cpf)
{
    if ((l != NULL) && (!lista_vazia(l)))
    {
        paciente_definir_cpf(l->inicio->pac, cpf);
        NO *aux = l->inicio->prox;

        while ((strcmp(paciente_obter_cpf(aux->pac), cpf) != 0))
        {
            aux = aux->prox;
        }

        // Se a busca parou em um nó que não é o cabeça, o paciente foi encontrado.
        if (aux != l->inicio)
            return aux->pac;
    }

    return NULL;
}

/**
 * @brief Imprime na tela os nomes de todos os pacientes na lista.
 * @param l Ponteiro para a lista.
 */
void lista_mostrar(LISTA *l)
{
    if ((l != NULL) && (!lista_vazia(l)))
    {
        NO *aux = l->inicio->prox;
        while (aux != l->inicio)
        {
            paciente_imprimir(aux->pac);
            aux = aux->prox;
        }
    }
}

/**
 * @brief Libera toda a memória associada à lista.
 * @details Desaloca a memória de cada paciente, de cada nó, do nó cabeça,
 * e finalmente da própria estrutura da lista.
 * @param l Ponteiro para o ponteiro da lista a ser apagada.
 */
void lista_apagar(LISTA **l)
{
    if (l == NULL || *l == NULL)
    {
        return;
    }

    NO *no_atual = (*l)->inicio->prox;
    NO *no_proximo;

    // Itera e libera cada nó e seu respectivo paciente
    while (no_atual != (*l)->inicio)
    {
        no_proximo = no_atual->prox;
        paciente_apagar(&(no_atual->pac));
        free(no_atual);
        no_atual = no_proximo;
    }

    // Libera o paciente e o nó cabeça
    paciente_apagar(&((*l)->inicio->pac));
    free((*l)->inicio);

    // Libera a estrutura da lista e anula o ponteiro original
    free(*l);
    *l = NULL;
}