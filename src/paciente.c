#include "../include/paciente.h"
#include "../include/historico.h"
#include <stdio.h>
#include <string.h>

struct paciente_{
    char* nome;
    char* cpf;
    HISTORICO* hist;
};

PACIENTE* paciente_criar(char nome[], char cpf[]){

    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p == NULL) return NULL;

    p->nome = NULL;
    p->cpf = NULL;
    p->hist = NULL;

    // 2. Aloca memória para o nome e copia
    p->nome = (char*)malloc(strlen(nome) + 1);
    if (p->nome == NULL) {
        paciente_apagar(&p);
        return NULL;
    }
    strcpy(p->nome, nome);

    // 3. Aloca memória para o CPF e copia 
    p->cpf = (char*)malloc(strlen(cpf) + 1);
    if (p->cpf == NULL) {
        paciente_apagar(&p);
        return NULL;
    }
    strcpy(p->cpf, cpf);

    // 4. Cria o histórico
    p->hist = historico_criar();
    if (p->hist == NULL) {
        paciente_apagar(&p);
        return NULL;
    }
    
    return p;
}

bool paciente_apagar(PACIENTE** paciente){
    // printf("Paciente apagar\n");
    if (paciente != NULL && (*paciente) != NULL){
        // printf("Paciente apagar e não é nulo\n");
        // printf("Histórico do paciente é NULL? %d\n", (*paciente)->hist == NULL);
        historico_apagar((&(*paciente)->hist));
        free((*paciente)->nome);
        free(*paciente);
        *paciente = NULL;
        return true;
    }

    return false;
}

char* paciente_obter_nome(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->nome;
    }

    return NULL;
}

char* paciente_obter_cpf(PACIENTE* paciente){
    if (paciente != NULL){
        // printf("PACICPF\n");
        return paciente->cpf;
    }

    return NULL;
}

void paciente_definir_cpf(PACIENTE* paciente, char cpf[]){
    if (paciente != NULL){
        strcpy(paciente->cpf, cpf);
    }
}


HISTORICO* paciente_obter_historico(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->hist;
    }

    return NULL;
}

char* paciente_para_string(PACIENTE* paciente, int* tamanho)
{
  int tam_nome = strlen(paciente->nome);
  int tam_historico = historico_tamanho(paciente->hist);

  HISTORICO* historico_aux = historico_criar();
  if (historico_aux == NULL) return NULL;

  int tamanho_total = 12 + tam_nome + 1;

  for (int i = 0; i < tam_historico; i++)
  {
    char* procedimento = historico_remover(paciente->hist);
    tamanho_total += strlen(procedimento) + 1; // +1 para o separador '\0'
    historico_inserir(historico_aux, procedimento);
  }

  char* str_paciente = calloc(tamanho_total, sizeof(char));

  if (str_paciente == NULL)
  {
      for (int i = 0; i < tam_historico; i++)
          historico_inserir(paciente->hist, historico_remover(historico_aux));
      historico_apagar(&historico_aux);
      return NULL;
  }

  char* ponteiro_atual = str_paciente;

  // Copia o CPF (11 bytes) e adiciona o separador '\0'
  memcpy(ponteiro_atual, paciente->cpf, 11);

  ponteiro_atual[11] = '\0';

  ponteiro_atual += 12; // Avança o ponteiro 12 posições (11 do cpf + 1 do \0)

  // Copia o nome. sprintf já adiciona o '\0'
  int bytes_escritos = sprintf(ponteiro_atual, "%s", paciente->nome);
  ponteiro_atual += bytes_escritos + 1; // Avança o ponteiro (+1 para pular o \0)

  // Esvazia a pilha auxiliar, escrevendo na string e restaurando a pilha original
  for (int i = 0; i < tam_historico; i++)
  {
    char* procedimento = historico_remover(historico_aux);
    
    bytes_escritos = sprintf(ponteiro_atual, "%s", procedimento);
    ponteiro_atual += bytes_escritos + 1;
    
    historico_inserir(paciente->hist, procedimento);
  }

  historico_apagar(&historico_aux);

  *tamanho = tamanho_total;
  return str_paciente;
}

// Função auxiliar para "deserializar"
PACIENTE* paciente_de_string(char* buffer)
{
  char cpf[12];
  char nome[256];
  
  // Lê o CPF
  strcpy(cpf, buffer);
  buffer += strlen(buffer) + 1; // Pula o CPF e seu \0

  // Lê o Nome
  strcpy(nome, buffer);
  buffer += strlen(nome) + 1; // Pula o Nome e seu \0

  PACIENTE* p = paciente_criar(nome, cpf);

  // Lê o histórico
  while (*buffer != '\0')
  {
    historico_inserir(p->hist, buffer); // Assumindo que inserir faz uma cópia da string
    buffer += strlen(buffer) + 1;
  }
  
  return p;
}

void paciente_imprimir(PACIENTE* paciente){
    if (paciente != NULL) printf("%s\n",paciente_obter_nome(paciente));
}
