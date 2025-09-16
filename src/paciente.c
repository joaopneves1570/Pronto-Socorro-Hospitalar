#include "paciente.h"

struct paciente_{
    char* nome;
    int id;
};

PACIENTE* paciente_criar(char nome[], int id){
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL){
        p->nome = (char*)malloc(sizeof(char)*(strlen(nome) + 1));

        if (p->nome != NULL){
            strcpy(p->nome, nome);
            p->id = id;
            return p;
        } 
    }
    free(p);
    return NULL;
}

bool paciente_apagar(PACIENTE** paciente){
    if ((*paciente) != NULL){
        free((*paciente)->nome);
        free(*paciente);
        *paciente = NULL;
        return true;
    }

    return false;
}

char* paciente_get_nome(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->nome;
    }

    return NULL;
}

int paciente_get_id(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->id;
    }

    return -1;
}

void paciente_imprimir(PACIENTE* paciente){
    if (paciente != NULL){
        printf("Paciente: %s\nID: %d\n", paciente->nome, paciente->id);
    }
}