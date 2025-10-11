#include "../include/IO.h"
#include "../include/paciente.h"
#include "../include/lista.h"
#include <stdio.h>
#include <string.h>

bool SAVE(LISTA **lista, FILA **fila)
{
  if (!lista || !(*lista) || !fila || !*(fila)) return false;
  
  PACIENTE* paciente; // Variável auxiliar 
  char* str_paciente;
  int tamanho_str_paciente;

  // Salvando os itens da fila

  FILE *fp_fila = fopen("fila_itens.bin", "wb");
  if (!fp_fila) return false;

  // Se mantém no while enquanto a fila não estiver vazia
  while((paciente = fila_remover(*fila)))
  {
    str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

    // printf("Oieeee\n");
    // Escreve a chave no arquivo
    fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_fila);
    fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_fila);
    // printf("fwrite\n");

    free(str_paciente);
    str_paciente = NULL;
  }

  // printf("Passou do while\n");

  // printf("Fechou o arquivo\n");
  fclose(fp_fila); fp_fila = NULL;
  // Libera memória
  // printf("Apagou a fila\n");
  // printf("A fila é NULL? %d\n", fila == NULL);
  fila_apagar(fila);

  // Salvando os itens da lista

  FILE *fp_lista = fopen("lista_itens.bin", "wb");

  if(!fp_lista) return false;

  // Se mantém no while enquanto a lista não estiver vazia
  while((paciente = lista_remover_inicio(*lista)))
  {
    str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);
    // Escreve a chave no arquivo
    fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_lista);
    fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_lista);
    // Apaga o item removido
    paciente_apagar(&paciente);

    free(str_paciente);
    str_paciente = NULL;
  }
  // Libera memória
  fclose(fp_lista); fp_lista = NULL;
  lista_apagar(lista);

  return true;
}

bool LOAD(LISTA **lista, FILA **fila)
{
  if (!*lista || !*fila) return false;

  // Carregando os itens do arquivo na lista

  FILE *fp_lista = fopen("lista_itens.bin", "rb");

  if (!fp_lista) return false;

  int tamanho_str_paciente;

  // Lê as chaves até o fim do arquivo
  while(fread(&tamanho_str_paciente, sizeof(int), 1, fp_lista) == 1)
  {
    char* buffer = calloc(tamanho_str_paciente + 1, sizeof(char));
    // Lê o BLOCO DE DADOS para o buffer
    fread(buffer, sizeof(char), tamanho_str_paciente, fp_lista);

    PACIENTE* paciente = paciente_de_string(buffer);
    lista_inserir(*lista, paciente);

    free(buffer);
  }

  fclose(fp_lista); // Libera memória

  // Carregando os itens do arquivo na fila

  FILE *fp_fila = fopen("fila_itens.bin", "rb");

  if (!fp_fila) return false;

  // Lê as chaves até o fim do arquivo
  while(fread(&tamanho_str_paciente, sizeof(int), 1, fp_fila) == 1)
  {
    char* buffer = calloc(tamanho_str_paciente + 1, sizeof(char));
    // Lê o BLOCO DE DADOS para o buffer
    fread(buffer, sizeof(char), tamanho_str_paciente, fp_fila);

    PACIENTE* paciente = paciente_de_string(buffer);
    fila_inserir(*fila, paciente);

    free(buffer);
  }

  fclose(fp_fila); // Libera memória

  return true;
}
