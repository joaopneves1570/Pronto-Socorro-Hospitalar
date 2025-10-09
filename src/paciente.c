#include "../include/paciente.h"
#include "../include/historico.h"
#include <stdio.h>
#include <string.h>

struct paciente_{
    char* nome;
    char cpf[12];
    HISTORICO* hist;
};

PACIENTE* paciente_criar(char nome[], char cpf[]){
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL){
        p->nome = (char*)malloc(sizeof(char)*(strlen(nome) + 1));

        if (p->nome != NULL){
            strcpy(p->nome, nome);
            strcpy(p->cpf, cpf);
            p->hist = historico_criar();
            return p;
        } 
    }
    free(p);
    return NULL;
}

bool paciente_apagar(PACIENTE** paciente){
    if ((*paciente) != NULL){
        historico_apagar((&(*paciente)->hist));
        free((*paciente)->nome);
        free(*paciente);
        *paciente = NULL;
        return true;
    }

    return false;
}

char* paciente_obter_nome(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->nome;
    }

    return NULL;
}

char* paciente_obter_cpf(PACIENTE* paciente){
    if (paciente != NULL){
        printf("PACICPF\n");
        return paciente->cpf;
    }

    return NULL;
}

void paciente_definir_cpf(PACIENTE* paciente, char cpf[]){
    if (paciente != NULL){
        strcpy(paciente->cpf, cpf);
    }
}


HISTORICO* paciente_obter_historico(PACIENTE* paciente){
    if (paciente != NULL){
        return paciente->hist;
    }

    return NULL;
}

void paciente_imprimir(PACIENTE* paciente){
    if (paciente != NULL){
    // TODO: Melhorar o print do CPF
        printf("Paciente: %s\nCPF: %s\n\n",paciente_obter_nome(paciente), paciente_obter_cpf(paciente));
    }
}
