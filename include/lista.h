#ifndef LISTA_H
    #define LISTA_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include "paciente.h"
    #include "historico.h"

    typedef struct lista_ LISTA;

    LISTA* lista_criar();

    bool lista_inserir(LISTA* l, PACIENTE* p);
    bool lista_remover(LISTA* l, PACIENTE* p);
    PACIENTE* lista_remover_inicio(LISTA* l);
    bool lista_vazia(LISTA* l);
    bool lista_cheia(LISTA* l); 

    PACIENTE* lista_buscar(LISTA* l, char* cpf);
    
    void lista_mostrar(LISTA* l);
    void lista_apagar(LISTA** l);

#endif
