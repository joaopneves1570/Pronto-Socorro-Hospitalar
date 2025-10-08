#include "include/IO.h"
#include "include/fila.h"
#include "include/historico.h"
#include "include/lista.h"
#include "include/paciente.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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

Opcao escolher_opcao()
{
  Opcao opcao_escolhida;
  do
  {
    scanf("%d", &opcao_escolhida);
    if (opcao_escolhida < 1 || opcao_escolhida > 8) printf("Opção inválida! (1-8)\n");
  }
  while (opcao_escolhida < 1 || opcao_escolhida > 8);

  return opcao_escolhida;
}

void formatar_cpf(char* cpf)
{
  int estranhos = 0;
  for (int i = 0; i < 15; i++)
  {
    if (isdigit(cpf[i]))
    {
      cpf[i - estranhos] = cpf[i];
    }
  }
  cpf[11] = '\0';
}

bool eh_cpf_valido(char* cpf)
{
  formatar_cpf(cpf);

  for (int i = 0; i < 11; i++) if (!isdigit(cpf[i])) return false;

  if (cpf[11] != '\0') return false;

  int soma = 0, dv[2];

  for (int i = 0; i < 9; i++) soma += (cpf[i] - 48) * (10 - i);

  dv[0] = soma % 11 > 1 ? 11 - soma % 11 : 0;

  if (cpf[9] - 48 != dv[0]) return false;

  soma = 0;

  for (int i = 0; i < 10; i++) soma += (cpf[i] - 48) * (11 - i);

  dv[1] = soma % 11 > 1 ? 11 - soma % 11 : 0;
  
  bool eh_valido = cpf[10] - 48 != dv[1];

  if (!eh_valido) printf("CPF inválido!");

  return eh_valido;
}

char* cpf_ler()
{
  char* cpf = calloc(16, sizeof(char));

  if (cpf == NULL) return NULL;

  printf("Digite o cpf: ");
  scanf("%15s", cpf);
  printf("\n");

  return eh_cpf_valido(cpf) ? cpf : NULL;
}

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
  LISTA* lista;
  FILA* fila;

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

  for (int i = 0; i < 8; i++) printf("%s\n", opcoes[i]);

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
          printf("Digite o nome do paciente: ");
          char nome[256]; scanf("%255[^\n]", nome);
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
          printf("Escreva o procedimento: ");
          char procedimento[256]; scanf("%255[^\n]", procedimento);
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
