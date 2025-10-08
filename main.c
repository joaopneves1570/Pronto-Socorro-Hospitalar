#include "include/IO.h"
#include "include/fila.h"
#include "include/historico.h"
#include "include/lista.h"
#include "include/paciente.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Opções do menu principal
typedef enum
{
  REGISTRAR_PACIENTE = 1,
  REGISTRAR_OBITO_DE_PACIENTE = 2,
  ADICIONAR_PROCEDIMENTO_AO_HISTORICO_MEDICO = 3,
  DESFAZER_PROCEDIMENTO_DO_HISTORICO_MEDICO = 4,
  CHAMAR_PACIENTE_PARA_ATENDIMENTO = 5,
  MOSTRAR_LISTA_DE_ESPERA = 6,
  MOSTRAR_HISTORICO_DO_PACIENTE = 7,
  SAIR = 8,
} Opcao;

/**
* @brief Lê o input do usuário, repetindo e avisando caso for inválido
*
* @return a enum correspondente à opção escolhida
*/
Opcao escolher_opcao()
{
  Opcao opcao_escolhida;

  // Repete a tentativa de escolha até uma escolha válida
  do
  {
    scanf("%d", &opcao_escolhida);
    getchar();

    printf("\n");

    // Confere se a opção escolhida pelo usuário está no intervalo válido
    if (opcao_escolhida < 1 || opcao_escolhida > 8) printf("Opção inválida! (1-8)\n");
  }
  while (opcao_escolhida < 1 || opcao_escolhida > 8);

  return opcao_escolhida;
}

/**
* @brief Altera a string formatando-a como xxxxxxxxxxxx + \0 + lixo
*
* @param cpf chave única do paciente
*/
void formatar_cpf(char cpf[])
{
  int estranhos = 0;

  // Itera sobre os caracteres deslocando cada dígito para esquerda de acordo com o número de caracteres estranhos encontrados
  for (int i = 0; i < 15; i++)
  {
    if (isdigit(cpf[i])) cpf[i - estranhos] = cpf[i];
    else estranhos++;
  }

  cpf[11] = '\0';
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

  // É inválido se há caracteres estranhos na string
  for (int i = 0; i < 11; i++) if (!isdigit(cpf[i])) goto invalido;

  // É inválido se não terminar onde deveria
  if (cpf[11] != '\0') goto invalido;

  // Algoritmo da verificação de CPF
  int soma = 0, dv[2];

  // Primeiro dígito verificador
  for (int i = 0; i < 9; i++) soma += (cpf[i] - '0') * (10 - i);

  dv[0] = soma % 11 > 1 ? 11 - soma % 11 : 0;

  // Confere se o primeiro dígito verificador fornecido é o que deveria ser de acordo com o algoritmo
  if (cpf[9] - '0' != dv[0]) goto invalido;

  soma = 0;

  // Segundo dígito verificador
  for (int i = 0; i < 10; i++) soma += (cpf[i] - '0') * (11 - i);

  dv[1] = soma % 11 > 1 ? 11 - soma % 11 : 0;
  
  // Confere se o segundo dígito verificador fornecido é o que deveria ser de acordo com o algoritmo
  if (cpf[10] - '0' != dv[1]) goto invalido;

  // Se não falhar em nenhum teste, retorna verdadeiro

  return true;

  // Se falhar em algum teste, avisa o usuário e retorna falso
invalido:

  printf("CPF inválido!");

  return false;
}

/**
* @brief Pede que o usuário digite um CPF, aloca a string dinamicamente para recebe-lo e testa se é válido
*
* @return um ponteiro para char de 15 posições alocado dinamicamente se válido, NULL caso contrário
*/ 
char* cpf_ler()
{
  // Tenta alocar o string
  char* cpf = calloc(15, sizeof(char));

  // Se falhar retorna um ponteiro nulo
  if (cpf == NULL) return NULL;

  // Mostra a instrução TODO:: Também confere se é valido, att a descrição
  printf("Digite o cpf: ");
  fgets(cpf, 16, stdin);
  cpf[strcspn(cpf, "\n")] = '\0';
  printf("\n");

  return eh_cpf_valido(cpf) ? cpf : NULL;
}

/**
* @brief Tenta obter um paciente da lista de acordo com um CPF que o cliente digitará
*
* @param lista a lista de pacientes
* @return o paciente com o CPF fornecido pelo cliente, NULL caso algo dê errado
*/
PACIENTE* paciente_ler(LISTA* lista)
{
  PACIENTE* paciente = NULL;
  char* cpf = cpf_ler();

  if (cpf) paciente = lista_buscar(lista, cpf);

  free(cpf);

  return paciente;
}

int main()
{
  LISTA* lista = lista_criar();
  FILA* fila = fila_criar();

  if (!LOAD(&lista, &fila)) return -1;

  char opcoes[][64] =
    {
      {"1. Registrar paciente"},
      {"2. Registrar óbito de paciente"},
      {"3. Adicionar procedimento ao histórico médico"},
      {"4. Desfazer procedimento do histórico médico"},
      {"5. Chamar paciente para atendimento"},
      {"6. Mostrar fila de espera"},
      {"7. Mostrar histórico do paciente"},
      {"8. Sair"},
    };

  Opcao opcao_escolhida;

  do
  {
    for (int i = 0; i < 8; i++) printf("%s\n", opcoes[i]);
    printf("Escolha uma das opções acima: ");
    opcao_escolhida = escolher_opcao();
    printf("\n");

    switch (opcao_escolhida)
    {
      case REGISTRAR_PACIENTE:
      {
        char* cpf = cpf_ler();
    
        if (cpf == NULL) break;

        PACIENTE* paciente = lista_buscar(lista, cpf);

        if (paciente) printf("Paciente já cadastrado.\n");
        else
        {
          char nome[256];

          printf("Digite o nome do paciente: ");
          fgets(nome, sizeof(nome), stdin);
          nome[strcspn(nome, "\n")] = '\0';
          printf("\n");

          paciente = paciente_criar(nome, cpf);

          lista_inserir(lista, paciente);
          printf("Paciente cadastrado com sucesso!\n");
        }

        if (fila_inserir(fila, paciente)) printf("Paciente adicionado à fila com sucesso!\n");

        free(cpf);

        break;
      }
      case REGISTRAR_OBITO_DE_PACIENTE:
      {
        PACIENTE* paciente = paciente_ler(lista);

        if (paciente)
        {
          lista_remover(lista, paciente);
          paciente_apagar(&paciente);
          
          printf("Registro do paciente removido de acordo com a LGPDb.\n");
        }
        else printf("Paciente não encontrado!\n");

        break;
      }
      case ADICIONAR_PROCEDIMENTO_AO_HISTORICO_MEDICO:
      {
        PACIENTE* paciente = paciente_ler(lista);

        if (paciente)
        {
          char procedimento[256];

          printf("Escreva o procedimento: ");
          fgets(procedimento, sizeof(procedimento), stdin);
          procedimento[strcspn(procedimento, "\n")] = '\0';
          printf("\n");

          HISTORICO* historico = paciente_obter_historico(paciente);

          if (historico)
          {
            if (historico_inserir(historico, procedimento)) printf("Procedimento adicionado ao histórico do paciente.\n");
            else printf("O procedimento não pôde ser adicionado ao histórico do paciente.\n");
          }
          else printf("Falha ao obter o histórico do paciente.\n");
        }
        else printf("O paciente não foi encontrado.\n");

        break;
      }
      case DESFAZER_PROCEDIMENTO_DO_HISTORICO_MEDICO:
      {
        PACIENTE* paciente = paciente_ler(lista);

        if (paciente)
        {
          HISTORICO* historico = paciente_obter_historico(paciente);

          if (historico)
          {
            char* procedimento = historico_remover(historico);
            if (procedimento)
            {
              printf("O seguinte procedimento retirado do histórico do paciente:\n%s\n", procedimento);
              free(procedimento);
            }
            else printf("O último procedimento não pôde ser removido do histórico do paciente.\n");
          }
          else printf("Falha ao obter o histórico do paciente.\n");
        }
        else printf("O paciente não foi encontrado.\n");

        break;
      }
      case CHAMAR_PACIENTE_PARA_ATENDIMENTO:
      {
        if (fila_vazia(fila))
        {
          printf("A fila está vazia.\n");
          break;
        }

        PACIENTE* paciente = fila_remover(fila);

        if (paciente) printf("O paciente %s é o próximo a ser atendido.\n", paciente_obter_nome(paciente));
        else printf("O paciente não foi encontrado.\n");

        break;
      

      case MOSTRAR_LISTA_DE_ESPERA:
        fila_imprimir(fila);

        break;
      }
      case MOSTRAR_HISTORICO_DO_PACIENTE:
      {
        PACIENTE* paciente = paciente_ler(lista);

        if (paciente)
        {
          HISTORICO* historico = paciente_obter_historico(paciente);

          if (historico) historico_imprimir(historico);
          else printf("Falha ao obter o histórico do paciente.\n");
        }
        else printf("O paciente não foi encontrado.\n");

        break;
      }
      case SAIR: break;
    }
  }
  while (opcao_escolhida != SAIR);

  SAVE(lista, fila);

  lista_apagar(&lista);
  fila_apagar(&fila);
}
