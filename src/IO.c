#include "../include/IO.h"
#include "../include/paciente.h"
#include "../include/lista.h"
#include <stdio.h>
#include <string.h>

bool SAVE(LISTA **lista, FILA **fila)
{
  if (!lista || !(*lista) || !fila || !*(fila)) return false;
  
  PACIENTE* paciente; // Variável auxiliar 
  char cpf[12];

  // Salvando os itens da fila

  FILE *fp_fila = fopen("fila_itens.bin", "wb");
  if (!fp_fila) return false;

  // Se mantém no while enquanto a fila não estiver vazia
  while((paciente = fila_remover(*fila)))
  {
    printf("Oieeee\n");
    // Escreve a chave no arquivo
    strcpy(cpf, paciente_obter_cpf(paciente));
    printf("strcpy\n");
    fwrite(cpf, sizeof(char), 12, fp_fila);
    printf("fwrite\n");
  }

  printf("Passou do while\n");

  printf("Fechou o arquivo\n");
  fclose(fp_fila); fp_fila = NULL;
  // Libera memória
  printf("Apagou a fila\n");
  printf("A fila é NULL? %d\n", fila == NULL);
  fila_apagar(fila);

  // Salvando os itens da lista

  FILE *fp_lista = fopen("lista_itens.bin", "wb");

  if(!fp_lista) return false;

  // Se mantém no while enquanto a lista não estiver vazia
  while((paciente = lista_remover_inicio(*lista)))
  {
    // Escreve a chave no arquivo
    strcpy(cpf, paciente_obter_cpf(paciente));
    fwrite(cpf, sizeof(char), 12, fp_lista);
    // Apaga o item removido
    paciente_apagar(&paciente);
  }
  // Libera memória
  fclose(fp_lista); fp_lista = NULL;
  lista_apagar(lista);

  return true;
}

bool LOAD(LISTA **lista, FILA **fila)
{
  if (!*lista || !*fila) return false;

  char cpf[12]; // Variável auxiliar

  // Carregando os itens do arquivo na lista

  FILE *fp_lista = fopen("lista_itens.bin", "rb");

  if (!fp_lista) return false;

  // Lê as chaves até o fim do arquivo
  while(fread(cpf, sizeof(char) * 12, 1, fp_lista) == 1)
  {
    cpf[11] = '\0';
    PACIENTE* paciente = paciente_criar("", cpf);
    lista_inserir(*lista, paciente);
  }

  fclose(fp_lista); // Libera memória

  // Carregando os itens do arquivo na fila

  FILE *fp_fila = fopen("fila_itens.bin", "rb");

  if (!fp_fila) return false;

  // Lê as chaves até o fim do arquivo
  while(fread(cpf, sizeof(char) * 12, 1, fp_fila) == 1)
  {
    cpf[11] = '\0';
    PACIENTE* paciente = paciente_criar("", cpf);
    fila_inserir(*fila, paciente);
  }

  fclose(fp_fila); // Libera memória

  return true;
}
