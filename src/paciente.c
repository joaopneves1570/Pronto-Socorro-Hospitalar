#include "paciente.h"
#include "triagem.h"
#include "historico.h"

struct paciente_{
    char* nome;
    int id;
    bool registrado;
    HISTORICO* hist;
};

PACIENTE* paciente_criar(char nome[], int id, bool registrado){
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL){
        p->nome = (char*)malloc(sizeof(char)*(strlen(nome) + 1));

        if (p->nome == NULL){
            free(p);
            return NULL;
        }
        
        strcpy(p->nome, nome);
        p->id = id;
        p->registrado = registrado;
        p->hist = historico_criar();

        if (p->hist == NULL){
            free(p->nome);
            free(p);
            return NULL;
        }

        return p;
    }

    free(p);
    return NULL;
}

bool paciente_registrar_obito(PACIENTE** paciente){
    if ((*paciente) != NULL){
        historico_apagar((&(*paciente)->hist));
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

bool paciente_get_registro(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->registrado;
    }

    return false;
}

HISTORICO* paciente_get_historico(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->hist;
    }

    return NULL;
}

void paciente_imprimir(PACIENTE* paciente){
    if (paciente != NULL){
        char registrado[4];
        if (paciente_get_registro(paciente)){
            strcpy(registrado, "Sim");
        } else {
            strcpy(registrado, "Nao");
        }
        printf("Paciente: %s\nID: %d\nRegistrado: %s\n\n",paciente_get_nome(paciente), paciente_get_id(paciente), registrado);
    }
}