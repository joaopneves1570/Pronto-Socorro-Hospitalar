#ifndef PACIENTE_H
    #define PACIENTE_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    typedef struct paciente_ PACIENTE;

    PACIENTE* paciente_criar(char nome[], int id, bool registrado);
    bool paciente_registrar_obito(PACIENTE** paciente);
    char* paciente_get_nome(PACIENTE* paciente);
    int paciente_get_id(PACIENTE* paciente);    
    HISTORICO* paciente_get_historico(PACIENTE* paciente);
    void paciente_imprimir(PACIENTE* paciente);


#endif