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
    HISTORICO* paciente_obter_historico(PACIENTE* paciente);
    void paciente_imprimir(PACIENTE* paciente);


#endif
