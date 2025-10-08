#ifndef FILA_H
    #define FILA_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include "paciente.h"

    typedef struct fila_ FILA;

    FILA* fila_criar();
    PACIENTE* fila_remover(FILA* fila);
    
    bool fila_inserir(FILA* fila, PACIENTE* paciente);
    bool fila_cheia(FILA* fila);
    bool fila_vazia(FILA* fila);

    void fila_apagar(FILA** fila);
    void fila_mostrar(FILA* fila);
    


#endif
