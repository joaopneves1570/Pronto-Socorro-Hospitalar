#ifndef IO_H
	#define IO_H
    #include "fila.h"
    #include "lista.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>

    bool SAVE(LISTA **lista, FILA **fila); 
    bool LOAD(LISTA **lista, FILA **fila); 
	  
#endif
