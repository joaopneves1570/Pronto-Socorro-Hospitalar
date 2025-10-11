#include "../include/paciente.h"
#include "../include/historico.h"

/**
 * @brief Estrutura que representa um paciente.
 * * Armazena os dados principais de um paciente, incluindo seu nome, CPF (usado como
 * chave única) e um ponteiro para seu histórico médico de procedimentos.
 */
struct paciente_
{
    char *nome;
    char *cpf;
    HISTORICO *hist;
};

/**
 * @brief Aloca e inicializa uma nova estrutura PACIENTE com os dados fornecidos.
 * * Esta função cria dinamicamente um novo paciente, alocando memória para a 
 * estrutura e para as strings de nome e CPF. Também cria um histórico médico vazio para o paciente.
 * * @param nome String contendo o nome do paciente.
 * @param cpf String contendo o CPF do paciente.
 * @return Ponteiro para a estrutura PACIENTE alocada ou NULL em caso de erro de alocação.
 */
PACIENTE *paciente_criar(char nome[], char cpf[])
{
    PACIENTE *p = (PACIENTE *)malloc(sizeof(PACIENTE));
    if (p == NULL)
        return NULL;

    // Inicializa as variaveis do TAD
    p->nome = NULL;
    p->cpf = NULL;
    p->hist = NULL;

    // Aloca memória para o nome e copia
    p->nome = (char *)malloc(strlen(nome) + 1);
    if (p->nome == NULL)
    {
        paciente_apagar(&p);
        return NULL;
    }
    strcpy(p->nome, nome);

    // Aloca memória para o CPF e copia
    p->cpf = (char *)malloc(strlen(cpf) + 1);
    if (p->cpf == NULL)
    {
        paciente_apagar(&p);
        return NULL;
    }
    strcpy(p->cpf, cpf);

    // Cria o histórico
    p->hist = historico_criar();
    if (p->hist == NULL)
    {
        paciente_apagar(&p);
        return NULL;
    }

    return p;
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
        historico_apagar(&((*paciente)->hist));
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
    if (paciente != NULL)
    {
        return paciente->nome;
    }

    return NULL;
}

/**
 * @brief Obtém o CPF de um paciente.
 * * @param paciente Ponteiro para a estrutura PACIENTE.
 * @return Ponteiro para a string com o CPF do paciente ou NULL se o paciente for nulo.
 */
char *paciente_obter_cpf(PACIENTE *paciente)
{
    if (paciente != NULL)
    {
        return paciente->cpf;
    }

    return NULL;
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
HISTORICO *paciente_obter_historico(PACIENTE *paciente)
{
    if (paciente != NULL)
    {
        return paciente->hist;
    }

    return NULL;
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
    int tam_nome = strlen(paciente->nome);
    int tam_historico = historico_tamanho(paciente->hist);

    HISTORICO *historico_aux = historico_criar();
    if (historico_aux == NULL)
        return NULL;

    // Calcula o tamanho total necessário para o buffer
    int tamanho_total = 12 + tam_nome + 1; // 11 (CPF) + 1 ('\0') + tam_nome + 1 ('\0')

    // Itera sobre o histórico para somar o tamanho de cada procedimento
    for (int i = 0; i < tam_historico; i++)
    {
        char *procedimento = historico_remover(paciente->hist);
        tamanho_total += strlen(procedimento) + 1; // +1 para o '\0'
        historico_inserir(historico_aux, procedimento);
    }

    char *str_paciente = calloc(tamanho_total, sizeof(char));

    // Se a alocação falhar, volta o histórico original e retorna
    if (str_paciente == NULL)
    {
        for (int i = 0; i < tam_historico; i++)
            historico_inserir(paciente->hist, historico_remover(historico_aux));
        historico_apagar(&historico_aux);
        return NULL;
    }

    char *ponteiro_atual = str_paciente;

    // Copia o CPF (11 bytes) e adiciona o separador '\0'
    memcpy(ponteiro_atual, paciente->cpf, 11);
    ponteiro_atual[11] = '\0';
    ponteiro_atual += 12; // Avança o ponteiro 12 posições

    // Copia o nome e avança o ponteiro
    int bytes_escritos = sprintf(ponteiro_atual, "%s", paciente->nome);
    ponteiro_atual += bytes_escritos + 1;

    // Esvazia a pilha auxiliar, escrevendo na string e restaurando a pilha original
    for (int i = 0; i < tam_historico; i++)
    {
        char *procedimento = historico_remover(historico_aux);

        bytes_escritos = sprintf(ponteiro_atual, "%s", procedimento);
        ponteiro_atual += bytes_escritos + 1;

        historico_inserir(paciente->hist, procedimento);
    }

    historico_apagar(&historico_aux);

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

    // Lê o CPF
    strcpy(cpf, buffer);
    buffer += strlen(buffer) + 1; // Pula o CPF e seu '\0'

    // Lê o Nome
    strcpy(nome, buffer);
    buffer += strlen(nome) + 1; // Pula o Nome e seu '\0'

    PACIENTE *p = paciente_criar(nome, cpf);

    // Lê cada procedimento do histórico até o final do buffer
    while (buffer != NULL && *buffer != '\0')
    {
        historico_inserir(p->hist, buffer);
        buffer += strlen(buffer) + 1;
    }

    return p;
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