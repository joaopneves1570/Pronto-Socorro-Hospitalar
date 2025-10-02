#ifndef TRIAGEM_H
    #define TRIAGEM_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include "paciente.h"

    typedef struct triagem_ TRIAGEM;

    TRIAGEM* triagem_criar();
    PACIENTE* triagem_remover(TRIAGEM* tri);
    
    bool triagem_inserir(TRIAGEM* tri, PACIENTE* pac);
    bool triagem_cheia(TRIAGEM* tri);
    bool triagem_vazia(TRIAGEM* tri);

    void triagem_apagar(TRIAGEM** tri);
    void triagem_mostrar(TRIAGEM* tri);
    


#endif