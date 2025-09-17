#ifndef HISTORICO_H
    #define HISTORICO_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    typedef struct historico_ HISTORICO;

    HISTORICO* historico_criar(void);
    bool historico_inserir(HISTORICO* hist, char texto[]);
    bool historico_retirar(HISTORICO* hist);
    bool historico_consultar(HISTORICO* hist);
    bool historico_cheio(HISTORICO* hist);
    bool historico_vazio(HISTORICO* hist);
    void historico_apagar(HISTORICO** hist);


#endif