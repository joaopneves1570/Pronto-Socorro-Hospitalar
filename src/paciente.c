#include "../include/paciente.h"

#include <math.h>

/**
 * @brief Estrutura que representa um paciente.
 * * Armazena os dados principais de um paciente, incluindo seu nome, CPF (usado como
 * chave única) e um ponteiro para seu histórico médico de procedimentos.
 */
struct paciente_
{
  char *nome;
  char *cpf;
  SENHA* senha;
};

/**
 * @brief Aloca e inicializa uma nova estrutura PACIENTE com os dados fornecidos.
 * * Esta função cria dinamicamente um novo paciente, alocando memória para a 
 * estrutura e para as strings de nome e CPF. Também cria um histórico médico vazio para o paciente.
 * * @param nome String contendo o nome do paciente.
 * @param cpf String contendo o CPF do paciente.
 * @return Ponteiro para a estrutura PACIENTE alocada ou NULL em caso de erro de alocação.
 */
PACIENTE *paciente_criar(char nome[], char cpf[], SENHA* senha)
{
  PACIENTE *paciente = (PACIENTE *)malloc(sizeof(PACIENTE));
  if (paciente == NULL) return NULL;

  // Aloca memória para o nome e copia
  paciente->nome = (char *)malloc(strlen(nome) + 1);
  if (paciente->nome == NULL)
  {
    paciente_apagar(&paciente);
    return NULL;
  }
  strcpy(paciente->nome, nome);

  // Aloca memória para o CPF e copia
  paciente->cpf = (char *)malloc(strlen(cpf) + 1);
  if (paciente->cpf == NULL)
  {
    paciente_apagar(&paciente);
    return NULL;
  }
  strcpy(paciente->cpf, cpf);

  paciente->senha = senha;

  return paciente;
}

/**
 * @brief Libera toda a memória associada a um paciente.
 * * Desaloca a memória do histórico, do nome, do CPF e da própria estrutura do paciente.
 * Para segurança, o ponteiro original que aponta para o paciente é definido como NULL.
 * * @param paciente Ponteiro para o ponteiro da estrutura PACIENTE a ser apagada.
 * @return true se a liberação foi bem-sucedida, false caso contrário.
 */
bool paciente_apagar(PACIENTE **paciente)
{
  if (paciente != NULL && (*paciente) != NULL)
  {
    senha_apagar(&((*paciente)->senha));
    free((*paciente)->nome);
    free((*paciente)->cpf); 
    free(*paciente);
    *paciente = NULL;
    return true;
  }

  return false;
}

/**
 * @brief Obtém o nome de um paciente.
 * * @param paciente Ponteiro para a estrutura PACIENTE.
 * @return Ponteiro para a string com o nome do paciente ou NULL se o paciente for nulo.
 */
char *paciente_obter_nome(PACIENTE *paciente)
{
  return paciente != NULL ? paciente->nome : NULL;
}

/**
 * @brief Obtém o CPF de um paciente.
 * * @param paciente Ponteiro para a estrutura PACIENTE.
 * @return Ponteiro para a string com o CPF do paciente ou NULL se o paciente for nulo.
 */
char *paciente_obter_cpf(PACIENTE *paciente)
{
  return paciente != NULL ? paciente->cpf : NULL;
}

SENHA* paciente_obter_senha(PACIENTE *paciente)
{
  return paciente != NULL ? paciente->senha : NULL;
}

/**
 * @brief Define ou atualiza o CPF de um paciente.
 * * @param paciente Ponteiro para a estrutura PACIENTE.
 * @param cpf Nova string de CPF a ser copiada para o paciente.
 */
void paciente_definir_cpf(PACIENTE *paciente, char cpf[])
{
  if (paciente != NULL)
  {
    strcpy(paciente->cpf, cpf);
  }
}

/**
 * @brief Obtém o ponteiro para o histórico médico de um paciente.
 * * @param paciente Ponteiro para a estrutura PACIENTE.
 * @return Ponteiro para a estrutura HISTORICO do paciente ou NULL se o paciente for nulo.
 */
void paciente_definir_senha(PACIENTE *paciente, SENHA* senha)
{
  if (paciente != NULL)
  {
    paciente-> senha = senha;
  }
}

/**
 * @brief Serializa os dados de um paciente em uma única string de bytes.
 * * Converte todos os dados do paciente (CPF, nome e procedimentos do histórico)
 * em um buffer contínuo de memória. O formato é:
 * [11 bytes CPF]\0[Nome]\0[Procedimento1]\0[Procedimento2]\0... e por ai vai
 * * @param paciente Ponteiro para o paciente a ser serializado.
 * @param tamanho Ponteiro para um inteiro onde o tamanho total da string alocada será armazenado.
 * @return Ponteiro para a string alocada dinamicamente contendo os dados serializados ou NULL em caso de erro.
 */
char *paciente_para_string(PACIENTE *paciente, int *tamanho)
{
  unsigned int pos = senha_obter_posicao(paciente->senha);
  int tam_posicao = (pos == 0) ? 1 : (int)log10(pos) + 1;
  
  int tam_nome = strlen(paciente->nome);

  // Calcula o tamanho total necessário para o buffer
  // 11 (CPF) + 1 ('\0') + tam_nome + 1 ('\0') + 2 (prioridade) + tam_posicao + 1 ('\0')
  int tamanho_total = 11 + 1 + tam_nome + 1 + 2 + tam_posicao + 1;

  char *str_paciente = calloc(tamanho_total, sizeof(char));

  if (str_paciente == NULL) return NULL;

  char *ponteiro_atual = str_paciente;

  // 1. Copia o CPF
  memcpy(ponteiro_atual, paciente->cpf, 11);
  ponteiro_atual[11] = '\0';
  ponteiro_atual += 12; // Avança 12 bytes

  // 2. Copia o Nome (String)
  // Usa "%s" para garantir que é uma string literal
  int bytes_escritos = sprintf(ponteiro_atual, "%s", paciente->nome);
  ponteiro_atual += bytes_escritos + 1; // +1 para pular o \0 que o sprintf colocou

  // 3. Copia a Prioridade
  bytes_escritos = sprintf(ponteiro_atual, "%s", senha_obter_prioridade(paciente->senha));
  ponteiro_atual += bytes_escritos + 1;

  // 4. Copia a Posição (Unsigned Int)
  bytes_escritos = sprintf(ponteiro_atual, "%u", pos);
  ponteiro_atual += bytes_escritos + 1;

  *tamanho = tamanho_total;
  return str_paciente;
}

/**
 * @brief Deserializa uma string de bytes para uma nova estrutura PACIENTE.
 * * Função inversa de `paciente_para_string`. Lê um buffer de memória com o formato
 * específico e recria a estrutura PACIENTE correspondente.
 * * @param buffer A string de bytes contendo os dados serializados do paciente.
 * @return Ponteiro para a nova estrutura PACIENTE criada ou NULL em caso de erro.
 */
PACIENTE *paciente_de_string(char *buffer)
{
  char cpf[12];
  char nome[256];
  char prioridade[3];
  char posicao_str[20];

  // 1. Lê o CPF
  strcpy(cpf, buffer);
  buffer += strlen(buffer) + 1; // Pula o CPF e seu '\0'

  // 2. Lê o Nome
  strcpy(nome, buffer);
  buffer += strlen(buffer) + 1; // Pula o Nome e seu '\0'

  // 3. Lê a Prioridade
  // Na função anterior, gravamos isso logo após o nome
  strcpy(prioridade, buffer);
  buffer += strlen(buffer) + 1;

  // 4. Lê a Posição (que está como string)
  strcpy(posicao_str, buffer);
  buffer += strlen(buffer) + 1; 

  // Converte a string da posição de volta para unsigned int
  unsigned int posicao = (unsigned int)atoi(posicao_str); 

  return paciente_criar(nome, cpf, prioridade);
}

/**
 * @brief Imprime o nome do paciente no console.
 * * @param paciente Ponteiro para a estrutura PACIENTE a ser impressa.
 */
void paciente_imprimir(PACIENTE *paciente)
{
  if (paciente != NULL)
    printf("%s\n", paciente_obter_nome(paciente));
}
