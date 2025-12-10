
#ifndef FILA_H
	#define FILA_H
  	#include "paciente.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	typedef struct fila_ FILA;

	FILA *fila_criar(void);
	bool fila_inserir(FILA *fila, PACIENTE *paciente, int prioridade);
	PACIENTE *fila_remover(FILA *fila);
	PACIENTE *fila_remover_com_prioridade(FILA* fila, int* prioridade);
	PACIENTE *fila_buscar(FILA *fila, char cpf[]);
	void fila_apagar(FILA **fila);
	PACIENTE *fila_frente(FILA *fila);
	int fila_tamanho(FILA *fila);
	bool fila_vazia(FILA *fila);
	bool fila_cheia(FILA *fila);
	void fila_imprimir(FILA *fila);
	  
#endif
