#include "../include/fila.h"
#include "../include/paciente.h" 
#include <stdio.h>
#include <stdlib.h>

typedef struct no_ NO;

/**
 * @brief Estrutura de um nó da fila.
 * * Cada nó armazena um ponteiro para um paciente e um ponteiro para o próximo nó na fila,
 * formando uma lista encadeada.
 */
struct no_
{
  NO *prox;
  PACIENTE *paciente;
};

/**
 * @brief Estrutura da fila de pacientes.
 * * Gerencia uma fila implementada como uma lista encadeada. Contém ponteiros
 * para o início e o fim da fila, além de um contador para o tamanho atual.
 */
struct fila_
{
  NO *inicio;
  NO *fim;
  int tamanho;
};

/**
 * @brief Aloca e inicializa uma nova fila vazia.
 * @return Ponteiro para a estrutura FILA alocada ou NULL em caso de erro.
 */
FILA *fila_criar()
{
  FILA *fila = (FILA *)malloc(sizeof(FILA));
  if (fila != NULL)
  {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
  }

  return fila;
}

/**
 * @brief Insere um paciente no final da fila (enqueue).
 * * Cria um novo nó para o paciente e o adiciona ao fim da lista encadeada.
 * A operação falha se a fila estiver cheia ou se houver erro de alocação.
 * @param fila Ponteiro para a fila.
 * @param pac Ponteiro para o paciente a ser inserido.
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool fila_inserir(FILA *fila, PACIENTE *paciente)
{
  if ((fila != NULL) && (!fila_cheia(fila)))
  {
    NO *novo = (NO *)malloc(sizeof(NO));
    if (novo == NULL) return false; 

    novo->paciente = paciente;
    novo->prox = NULL;

    if (fila_vazia(fila))
      fila->inicio = novo;
    else
      fila->fim->prox = novo;

    fila->fim = novo;
    fila->tamanho++;
    return true;
  }

  return false;
}

/**
 * @brief Remove e retorna o paciente do início da fila (dequeue).
 * * Retorna o ponteiro para o paciente que estava no início da fila e libera
 * a memória do nó que o continha. Não libera a memória do paciente em si.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para o PACIENTE removido ou NULL se a fila estiver vazia.
 */
PACIENTE *fila_remover(FILA *fila)
{
  if ((fila != NULL) && (!fila_vazia(fila)))
  {
    NO *atual = fila->inicio;
    PACIENTE *pac = atual->paciente;

    fila->inicio = atual->prox;
    free(atual); 
    if (fila->inicio == NULL)
    {
      fila->fim = NULL;
    }

    fila->tamanho--;
    return pac;
  }

  return NULL;
}

/**
 * @brief Verifica se a fila atingiu sua capacidade máxima.
 * @param fila Ponteiro para a fila.
 * @return true se a fila está cheia, false caso contrário.
 */
bool fila_cheia(FILA *fila)
{
  if (fila != NULL)
  {
    NO* no = malloc(sizeof(NO));
    
    free(no);

    return no == NULL;
  }
  
  return true;
}

/**
 * @brief Verifica se a fila não contém nenhum paciente.
 * @param fila Ponteiro para a fila.
 * @return true se a fila está vazia, false caso contrário.
 */
bool fila_vazia(FILA *fila)
{
  if (fila != NULL)
  {
    return (fila->tamanho == 0);
  }
  return true;
}

/**
 * @brief Libera toda a memória associada à estrutura da fila.
 * * IMPORTANTE: Esta função libera apenas os nós da fila e a própria estrutura da fila.
 * Ela **NÃO** libera a memória dos pacientes, pois eles são gerenciados pela lista principal.
 * Apagar os pacientes aqui causaria um erro de "double-free" quando a lista tentasse apagá-los.
 * @param fila Ponteiro para o ponteiro da fila a ser apagada.
 */
void fila_apagar(FILA **fila)
{
  if (fila == NULL || *fila == NULL)
  {
    return;
  }

  NO *atual = (*fila)->inicio;
  NO *proximo = NULL;

  // Percorre a fila liberando cada nó
  while (atual != NULL)
  {
    proximo = atual->prox;
    free(atual);
    atual = proximo;
  }

  // Libera a estrutura da fila e define o ponteiro original como NULL
  free(*fila);
  *fila = NULL;
}

/**
 * @brief Busca um paciente na fila pelo seu CPF sem removê-lo.
 * @param fila Ponteiro para a fila.
 * @param cpf String com o CPF a ser buscado.
 * @return Ponteiro para o PACIENTE encontrado ou NULL se não for encontrado.
 */
PACIENTE *fila_buscar(FILA *fila, char cpf[])
{
  if (fila && !fila_vazia(fila))
  {
    NO *aux = fila->inicio;
    while (aux != NULL)
    {
      // Compara o CPF do paciente atual com o CPF buscado
      if (strcmp(paciente_obter_cpf(aux->paciente), cpf) == 0)
      {
        return aux->paciente; // Retorna o paciente se encontrou
      }
      aux = aux->prox;
    }
  }

  return NULL; // Retorna NULL se a fila é nula, vazia ou o CPF não foi encontrado
}

/**
 * @brief Imprime na tela os nomes dos pacientes na fila de espera.
 * * Percorre a fila do início ao fim, imprimindo os dados de cada paciente
 * em sua ordem de chegada.
 * @param fila Ponteiro para a fila.
 */
void fila_imprimir(FILA *fila)
{
  if (fila != NULL)
  {
    if (fila_vazia(fila))
    {
      printf("A fila de espera está vazia.\n");
      return;
    }

    NO *atual = fila->inicio;
    int posicao = 1;
    printf("--- Fila de Espera ---\n");
    while(atual != NULL)
    {
      printf("%d. ", posicao++);
      paciente_imprimir(atual->paciente); // Imprime o nome do paciente
      atual = atual->prox;
    }
    printf("----------------------\n");
  }
}

FILA* fila_carregar(char* origem, LISTA* lista)
{
  FILA* fila = fila_criar();

  if (fila == NULL) return false;

  FILE* fp_fila = fopen(origem, "rb");
  if (fp_fila == NULL) return false;

  int tamanho_str_paciente;

  while (fread(&tamanho_str_paciente, sizeof(int), 1, fp_fila) == 1)
  {
    char *buffer = malloc((tamanho_str_paciente + 1) * sizeof(char));

    if (buffer == NULL)
    {
      fclose(fp_fila);
      return false;
    }

    fread(buffer, sizeof(char), tamanho_str_paciente, fp_fila);
    
    char cpf_temp[12];
    strncpy(cpf_temp, buffer, 11);
    cpf_temp[11] = '\0';
    
    PACIENTE* paciente = lista_buscar(lista, cpf_temp);

    if (paciente) fila_inserir(fila, paciente);

    free(buffer);
  }

  fclose(fp_fila);

  return true;
}

bool fila_salvar(FILA* fila, char* destino)
{
  if (fila == NULL) return false;

  NO* it = fila->inicio;
  PACIENTE* paciente;
  char *str_paciente;
  int tamanho_str_paciente;

  FILE *fp_fila = fopen(destino, "wb");
  if (!fp_fila) return false;

  do
  {
    str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

    if (str_paciente)
    {
        fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_fila);
        fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_fila);
        free(str_paciente);
        str_paciente = NULL;
    }

    it = it->prox;
  }
  while (it != fila->inicio);

  fclose(fp_fila); 
  fp_fila = NULL;

  return true;
}
