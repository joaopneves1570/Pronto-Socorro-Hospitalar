#include "../include/senha.h"
#include <string.h>

/**
 * @brief Estrutura da pilha de histórico médico.
 * * Implementada como uma pilha (LIFO - Last-In, First-Out) usando uma lista encadeada.
 * Armazena um ponteiro para o topo da pilha e o tamanho atual.
 */
typedef struct senha_
{
  int prioridade;
  int posicao;
} SENHA;

/**
 * @brief Aloca e inicializa uma nova estrutura de histórico (pilha).
 * @return Ponteiro para a estrutura SENHA alocada ou NULL em caso de erro.
 */
SENHA* senha_criar(int prioridade, unsigned int posicao)
{
  SENHA *senha = (SENHA*)malloc(sizeof(SENHA));

  if (senha != NULL)
  {
    senha->prioridade = prioridade;
    senha->posicao = posicao;
    return senha;
  }

  return NULL;
}

/**
 * @brief Insere um novo procedimento no topo do histórico (operação push).
 * * Cria um novo nó, copia a string do procedimento para ele e o coloca no topo da pilha.
 * @param senha Ponteiro para o histórico.
 * @param procedimento String contendo a descrição do procedimento a ser adicionado.
 * @return true se a inserção foi bem-sucedida, false caso contrário (histórico cheio ou erro de alocação).
 */
bool senha_alterar(SENHA *senha, int prioridade, unsigned int posicao)
{
  if (senha != NULL)
  {
    senha->prioridade = prioridade;
    senha->posicao = posicao;
  }

  return senha != NULL;
}

/**
 * @brief Remove e retorna o procedimento mais recente do topo do histórico (operação pop).
 * @details Esta função aloca dinamicamente uma nova string para o procedimento removido.
 * O chamador é responsável por liberar essa memória com free().
 * @param senha Ponteiro para o histórico.
 * @return Ponteiro para uma nova string com o procedimento removido, ou NULL se o histórico estiver vazio.
 */
int senha_obter_prioridade(SENHA* senha)
{
  return senha != NULL ? senha->prioridade : -1;
}

/**
 * @brief Consulta o procedimento no topo do histórico sem removê-lo (operação top/peek).
 * @details O procedimento é copiado para o buffer de char fornecido pelo chamador.
 * @param senha Ponteiro para o histórico.
 * @param procedimento Ponteiro para um buffer de char onde o procedimento será copiado.
 * @return true se a consulta foi bem-sucedida, false se o histórico estiver vazio.
 */
unsigned int senha_obter_posicao(SENHA* senha)
{
  return senha != NULL ? senha->posicao : 0;
}

/**
 * @brief Libera toda a memória associada a um histórico.
 * @details Desaloca a memória de todos os nós da pilha e da própria estrutura de histórico.
 * O ponteiro original que aponta para o histórico é definido como NULL.
 * @param senha Ponteiro para o ponteiro do histórico a ser apagado.
 */
void senha_apagar(SENHA **senha)
{
  if (senha != NULL && *senha != NULL)
  {
    // free(senha->prioridade); // ?
    free(senha);
    *senha = NULL;
  }
}

char* prioridade_sigla(int i)
{
  switch (i)
  {
    case 0: return "EM";
    case 1: return "MU";
    case 2: return "UR";
    case 3: return "PU";
    case 4: return "NU";
  }
  return "";
}

/**
 * @brief Imprime todos os procedimentos do histórico, do mais recente para o mais antigo.
 * @param senha Ponteiro para o histórico a ser impresso.
 */
void senha_imprimir(SENHA *senha)
{
  if (senha != NULL)
  {
    printf("%s%d\n", prioridade_sigla(senha->prioridade), senha->posicao % 999 + 1);
  }
}
