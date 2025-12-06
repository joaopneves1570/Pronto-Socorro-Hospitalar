#ifndef SENHA_H
    #define SENHA_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    typedef struct senha_ SENHA;

    SENHA* senha_criar(char prioridade[], unsigned int posicao);
    bool senha_alterar(SENHA* senha, char prioridade[], unsigned int posicao);
    char* senha_obter_prioridade(SENHA* senha);
    unsigned int senha_obter_posicao(SENHA* senha);
    void senha_apagar(SENHA** senha);
    void senha_imprimir(SENHA* senha);

#endif
