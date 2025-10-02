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
    bool lista_vazia(LISTA* l);
    bool lista_cheia(LISTA* l); 

    int lista_buscar(LISTA* l, PACIENTE* p);
    
    void lista_mostrar(LISTA* l);
    void lista_apagar(LISTA** l);

#endif