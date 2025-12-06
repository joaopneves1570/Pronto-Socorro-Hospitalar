#include "include/IO.h"
#include "include/fila.h"
#include "include/lista.h"
#include "include/paciente.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


// Inclusões para limpar a tela de forma portável (Windows/Linux)
#ifdef _WIN32
#include <windows.h>
#define LIMPAR_TELA "cls"
#else
#include <unistd.h>
#define LIMPAR_TELA "clear"
#endif

// Definições de cores e estilos ANSI para o terminal
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN  "\x1b[36m"
#define ANSI_STYLE_BOLD  "\x1b[1m"

/**
 * @brief Limpa a tela do console.
 */
void limpar_tela()
{
  system(LIMPAR_TELA);
}

/**
 * @brief Pausa a execução e aguarda o usuário pressionar Enter.
 */
void pausar_para_continuar()
{
  printf("\n" ANSI_STYLE_BOLD "Pressione Enter para continuar..." ANSI_COLOR_RESET);
  // Limpa o buffer de entrada antes de esperar pelo Enter
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  getchar();
}

/**
 * @brief Imprime um cabeçalho padronizado e limpa a tela.
 * @param titulo O texto a ser exibido no cabeçalho.
 */
void imprimir_cabecalho(const char *titulo)
{
  limpar_tela();
  printf(ANSI_STYLE_BOLD ANSI_COLOR_CYAN "--- %s ---\n\n" ANSI_COLOR_RESET, titulo);
}

/**
 * @brief Exibe o menu principal de opções.
 * @param opcoes A matriz de strings com as opções do menu.
 */
void exibir_menu_principal(char opcoes[][64])
{
  limpar_tela();
  printf(ANSI_STYLE_BOLD ANSI_COLOR_CYAN "--- Sistema de Gestão de Pacientes ---\n\n" ANSI_COLOR_RESET);
  for (int i = 0; i < 8; i++)
  {
    printf("%s\n", opcoes[i]);
  }
  printf("\nEscolha uma das opções acima: ");
}

// Opções do menu principal
typedef enum
{
  REGISTRAR_PACIENTE = 1,
  REMOVER_PACIENTE = 2,
  LISTAR_PACIENTES = 3,
  BUSCAR_PACIENTE_POR_ID = 4,
  MOSTRAR_LISTA_DE_ESPERA = 5,
  DAR_ALTA_AO_PACIENTE = 6,
  SAIR = 7,
} Opcao;

/**
 * @brief Lê o input do usuário, repetindo e avisando caso for inválido
 *
 * @return a enum correspondente à opção escolhida
 */
Opcao escolher_opcao()
{
  int opcao_escolhida;
  Opcao opcao_escolhida_;

  // Repete a tentativa de escolha até uma escolha válida
  do
  {
    scanf("%d", &opcao_escolhida);
    getchar();

    printf("\n");

    // Confere se a opção escolhida pelo usuário está no intervalo válido
    if (opcao_escolhida < 1 || opcao_escolhida > 7)
      printf("Opção inválida! Escolha uma das opções acima (1-7): ");
  } while (opcao_escolhida < 1 || opcao_escolhida > 7);

  opcao_escolhida_ = (Opcao)opcao_escolhida;
  return opcao_escolhida_;
}

/**
 * @brief Altera a string formatando-a como xxxxxxxxxxxx + \0 + lixo
 *
 * @param cpf chave única do paciente
 */
void formatar_cpf(char cpf[])
{
  int tamanho = 0;

  // Itera sobre os caracteres deslocando cada dígito para esquerda de acordo com o número de caracteres estranhos encontrados
  for (int i = 0; cpf[i] != '\0'; i++)
  {
    // printf("%c", cpf[i]);
    if (isdigit(cpf[i]))
    {
      cpf[tamanho] = cpf[i];
      tamanho++;
    }
  }

  cpf[tamanho] = '\0';
}

/**
 * @brief formata o CPF e confere se ele é válido, avisando o usuário caso contrário
 *
 * @param cpf chave única do paciente
 * @return true se o CPF é valido, false caso contrário
 */
bool eh_cpf_valido(char cpf[])
{
  formatar_cpf(cpf);

  int digitos_iguais_ao_primeiro = 0;

  // É inválido se há caracteres estranhos na string
  for (int i = 0; i < 11; i++)
  {
    if (cpf[i] == cpf[0])
      digitos_iguais_ao_primeiro++;
    if (cpf[i] == '\0')
    {
      printf("CPF inválido! (Muito curto)");
      return false;
    }
    else if (!isdigit(cpf[i]))
    {
      printf("CPF inválido! (Caracteres estranhos)");
      return false;
    }
  }

  if (digitos_iguais_ao_primeiro == 11)
  {
    printf("CPF inválido! (Combinação proibida)");
    return false;
  }

  // Algoritmo da verificação de CPF
  int soma = 0, dv[2];

  // Primeiro dígito verificador
  for (int i = 0; i < 9; i++)
    soma += (cpf[i] - '0') * (10 - i);

  dv[0] = soma % 11 > 1 ? 11 - soma % 11 : 0;

  // Confere se o primeiro dígito verificador fornecido é o que deveria ser de acordo com o algoritmo
  if (cpf[9] - '0' != dv[0])
  {
    printf("CPF inválido! (Esse CPF não existe)");
    return false;
  }

  soma = 0;

  // Segundo dígito verificador
  for (int i = 0; i < 10; i++)
    soma += (cpf[i] - '0') * (11 - i);

  dv[1] = soma % 11 > 1 ? 11 - soma % 11 : 0;

  // Confere se o segundo dígito verificador fornecido é o que deveria ser de acordo com o algoritmo
  if (cpf[10] - '0' != dv[1])
  {
    printf("CPF inválido! (Esse CPF não existe)");
    return false;
  }

  // Se não falhar em nenhum teste, retorna verdadeiro
  return true;
}

/**
 * @brief Pede que o usuário digite um CPF, aloca a string dinamicamente para recebe-lo e testa se é válido
 *
 * @return um ponteiro para char de 15 posições alocado dinamicamente se válido, NULL caso contrário
 */
char *cpf_ler()
{
  // Tenta alocar o string
  char *cpf = calloc(16, sizeof(char));

  // Se falhar retorna um ponteiro nulo
  if (cpf == NULL)
    return NULL;

  // Mostra a instrução TODO:: Também confere se é valido, att a descrição
  printf("Digite o cpf: ");
  fgets(cpf, 16, stdin);
  cpf[14] = '\0';
  printf("\n");

  if (!eh_cpf_valido(cpf))
  {
    free(cpf);
    cpf = NULL;
  }

  // printf("CPF válido.\n");
  return cpf;
}

/**
 * @brief Tenta obter um paciente da lista de acordo com um CPF que o cliente digitará
 *
 * @param lista a lista de pacientes
 * @return o paciente com o CPF fornecido pelo cliente, NULL caso algo dê errado
 */
PACIENTE *paciente_ler(LISTA *lista)
{
  PACIENTE *paciente = NULL;
  char *cpf = cpf_ler();

  if (cpf)
    paciente = lista_buscar(lista, cpf);

  free(cpf);

  return paciente;
}

int main()
{
  LISTA *lista = lista_criar();
  FILA* filas[5];

  for (int i = 0; i < 5; i++)
  {
    filas[i] = fila_criar();
    if (filas[i] == NULL)
    {
      for (int j = 0; j < i; j++) fila_apagar(&filas[j]);
    }
    lista_apagar(&lista);
    return -1;
  }

  if (!LOAD(&lista, &fila))
  {
    printf(ANSI_COLOR_RED "[ERRO] Falha ao carregar os dados. O programa será encerrado.\n" ANSI_COLOR_RESET);
    return -1;
  }

  char opcoes[][64] =
    {
      {"1. Registrar paciente"},
      {"2. Remover paciente"},
      {"3. Listar pacientes"},
      {"4. Buscar paciente por ID"},
      {"5. Mostrar fila de espera"},
      {"6. Dar alta ao paciente"},
      {"7. Sair"},
    };

  Opcao opcao_escolhida;

  do
  {
    exibir_menu_principal(opcoes);
    opcao_escolhida = escolher_opcao();

    // Imprime um cabeçalho para a ação escolhida
    if (opcao_escolhida != SAIR)
    {
      imprimir_cabecalho(opcoes[opcao_escolhida - 1]);
    }

    switch (opcao_escolhida)
    {
    case REGISTRAR_PACIENTE:
    {
      char *cpf = cpf_ler();
      if (cpf == NULL)
        break; // A mensagem de erro de CPF já foi exibida em eh_cpf_valido

      PACIENTE *paciente = lista_buscar(lista, cpf);

      if (paciente)
      {
        printf(ANSI_COLOR_YELLOW "[INFO] Paciente com CPF %s já está cadastrado.\n" ANSI_COLOR_RESET, cpf);
      }
      else
      {
        char nome[256];
        printf("Digite o nome do paciente: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0';
        printf("\n");

        paciente = paciente_criar(nome, cpf);
        lista_inserir(lista, paciente);
        printf(ANSI_COLOR_GREEN "[SUCESSO] Paciente cadastrado com sucesso!\n" ANSI_COLOR_RESET);
      }

      if (fila_inserir(fila, paciente))
        printf(ANSI_COLOR_GREEN "[SUCESSO] Paciente adicionado à fila de espera!\n" ANSI_COLOR_RESET);

      free(cpf);
      break;
    }
    case REGISTRAR_OBITO_DE_PACIENTE:
    {
      PACIENTE *paciente = paciente_ler(lista);

      if (paciente)
      {
        if (fila_buscar(fila, paciente_obter_cpf(paciente)))
        {
          printf(ANSI_COLOR_YELLOW "[ALERTA] O paciente está na fila de espera e não pode ser removido.\n" ANSI_COLOR_RESET);
          break;
        }
        paciente = lista_remover(lista, paciente);
        paciente_apagar(&paciente);

        printf(ANSI_COLOR_GREEN "[SUCESSO] Registro do paciente removido permanentemente.\n" ANSI_COLOR_RESET);
      }
      else
        printf(ANSI_COLOR_RED "[ERRO] Paciente não encontrado!\n" ANSI_COLOR_RESET);

      break;
    }
    case ADICIONAR_PROCEDIMENTO_AO_HISTORICO_MEDICO:
    {
      PACIENTE *paciente = paciente_ler(lista);
      if (paciente)
      {
        char procedimento[256];
        printf("Descreva o procedimento realizado: ");
        fgets(procedimento, sizeof(procedimento), stdin);
        procedimento[strcspn(procedimento, "\n")] = '\0';
        printf("\n");

        HISTORICO *historico = paciente_obter_historico(paciente);
        if (historico_inserir(historico, procedimento))
          printf(ANSI_COLOR_GREEN "[SUCESSO] Procedimento adicionado ao histórico.\n" ANSI_COLOR_RESET);
        else
          printf(ANSI_COLOR_RED "[ERRO] Não foi possível adicionar o procedimento.\n" ANSI_COLOR_RESET);
      }
      else
        printf(ANSI_COLOR_RED "[ERRO] Paciente não encontrado.\n" ANSI_COLOR_RESET);

      break;
    }
    case DESFAZER_PROCEDIMENTO_DO_HISTORICO_MEDICO:
    {
      PACIENTE *paciente = paciente_ler(lista);
      if (paciente)
      {
        HISTORICO *historico = paciente_obter_historico(paciente);
        if (historico_vazio(historico))
        {
          printf(ANSI_COLOR_YELLOW "[INFO] O histórico do paciente está vazio. Nada a desfazer.\n" ANSI_COLOR_RESET);
        }
        else
        {
          char *procedimento = historico_remover(historico);
          if (procedimento)
          {
            printf(ANSI_COLOR_GREEN "[SUCESSO] O seguinte procedimento foi desfeito e removido do histórico:\n" ANSI_COLOR_RESET);
            printf("'%s'\n", procedimento);
            free(procedimento);
          }
          else
            printf(ANSI_COLOR_RED "[ERRO] Não foi possível remover o último procedimento.\n" ANSI_COLOR_RESET);
        }
      }
      else
        printf(ANSI_COLOR_RED "[ERRO] Paciente não encontrado.\n" ANSI_COLOR_RESET);
      break;
    }
    case CHAMAR_PACIENTE_PARA_ATENDIMENTO:
    {
      if (fila_vazia(fila))
      {
        printf(ANSI_COLOR_YELLOW "[INFO] A fila de espera está vazia.\n" ANSI_COLOR_RESET);
        break;
      }

      PACIENTE *paciente = fila_remover(fila);
      if (paciente)
        printf(ANSI_COLOR_GREEN "Próximo paciente a ser atendido: %s" ANSI_STYLE_BOLD " (%s)\n" ANSI_COLOR_RESET, paciente_obter_nome(paciente), paciente_obter_cpf(paciente));
      else
        printf(ANSI_COLOR_RED "[ERRO] Falha ao chamar o próximo paciente.\n" ANSI_COLOR_RESET);

      break;
    }
    case MOSTRAR_LISTA_DE_ESPERA:
    {
      if (fila_vazia(fila))
        printf(ANSI_COLOR_YELLOW "[INFO] A fila de espera está vazia.\n" ANSI_COLOR_RESET);
      else
        fila_imprimir(fila);

      break;
    }
    case MOSTRAR_HISTORICO_DO_PACIENTE:
    {
      PACIENTE *paciente = paciente_ler(lista);
      if (paciente)
      {
        HISTORICO *historico = paciente_obter_historico(paciente);
        printf("Histórico de " ANSI_STYLE_BOLD "%s" ANSI_COLOR_RESET ":\n", paciente_obter_nome(paciente));
        historico_imprimir(historico);
      }
      else
        printf(ANSI_COLOR_RED "[ERRO] Paciente não encontrado.\n" ANSI_COLOR_RESET);

      break;
    }
    case SAIR:
      imprimir_cabecalho("Saindo do Sistema");
      printf("Salvando todos os dados antes de encerrar...\n");
      break;
    }

    if (opcao_escolhida != SAIR)
    {
      pausar_para_continuar();
    }

  } while (opcao_escolhida != SAIR);

  SAVE(&lista, &fila);
  printf(ANSI_COLOR_GREEN "[SUCESSO] Dados salvos. Até logo!\n" ANSI_COLOR_RESET);

  lista_apagar(&lista);
  fila_apagar(&fila);

  return 0;
}
