#ifndef PACIENTE_H
    #define PACIENTE_H

    #include "historico.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    typedef struct paciente_ PACIENTE;

    PACIENTE* paciente_criar(char nome[], char cpf[]);
    bool paciente_apagar(PACIENTE** paciente);
    char* paciente_obter_nome(PACIENTE* paciente);
    char* paciente_obter_cpf(PACIENTE* paciente);    
    void paciente_definir_cpf(PACIENTE* paciente, char cpf[]);    
    HISTORICO* paciente_obter_historico(PACIENTE* paciente);
    void paciente_imprimir(PACIENTE* paciente);
    char* paciente_para_string(PACIENTE* paciente, int* tamanho);
    PACIENTE* paciente_de_string(char* buffer);
    bool paciente_esta_na_fila(PACIENTE* paciente);
    void paciente_ir_para_fila(PACIENTE* paciente);
    void paciente_sair_da_fila(PACIENTE* paciente);

#endif
