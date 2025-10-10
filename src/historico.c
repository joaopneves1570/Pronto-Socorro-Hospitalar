#include "../include/historico.h"
#define TAM_MAX 10

typedef struct no_ NO;

struct no_{
    NO* anterior;
    char procedimento[101];
};

struct historico_{
    NO* topo;
    int tamanho;
};

HISTORICO* historico_criar(void){
    HISTORICO* hist = (HISTORICO* )malloc(sizeof(HISTORICO));
    if (hist != NULL){
        hist->tamanho = 0;
        hist->topo = NULL;
        return hist;
    }

    return NULL;
}

bool historico_inserir(HISTORICO* hist, char procedimento[]){
    if (hist != NULL && !historico_cheio(hist)){
        NO* novo = (NO* )malloc(sizeof(NO));
        if (novo != NULL){
            if (strlen(procedimento) > 100){
                printf("Tamanho da string de procedimento muito grande.\n");
                printf("Por favor, insira um procedimento de até 100 letras.\n");
                return false;
            }
            strcpy(novo->procedimento, procedimento);
            novo->anterior = hist->topo;
            hist->topo = novo;
            hist->tamanho++;

            return true;
        }
    }
    return false;
}

char* historico_remover(HISTORICO* hist){
    if (hist != NULL && !historico_vazio(hist)){
        NO* aux = hist->topo;

        char* procedimento = calloc(101, sizeof(char));

        strcpy(procedimento, hist->topo->procedimento);

        hist->topo = aux->anterior;
        aux->anterior = NULL;
        free(aux); aux = NULL;

        hist->tamanho--;

        return procedimento;
    }
    return NULL;
}

bool historico_consultar(HISTORICO* hist, char* procedimento){
    if (hist != NULL && !historico_vazio(hist)){

        strcpy(procedimento, hist->topo->procedimento);

        return true;
    }
    return false;
}

bool historico_cheio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == TAM_MAX;
    }
    return false;
}

bool historico_vazio(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho == 0;
    }
    return false;
}

void historico_apagar(HISTORICO** hist){
    if ((*hist) != NULL && (!historico_vazio(*hist))){
        while ((*hist)->topo != NULL){
            NO* aux = (*hist)->topo;
            (*hist)->topo = aux->anterior;
            aux->anterior = NULL;
            free(aux); aux = NULL;
        }
    }
    free(*hist); (*hist) = NULL;
}

void historico_imprimir(HISTORICO* hist){
    if (hist != NULL){
        if (historico_vazio(hist))
        {
            printf("O histórico do paciente está vazio.\n");
            return;
        }
        for (NO* no = hist->topo; no != NULL; no = no->anterior)
        {
            printf("- %s\n", no->procedimento);
        }
    }
}

int historico_tamanho(HISTORICO* hist){
    if (hist != NULL){
        return hist->tamanho;
    }

    return -1;
}

bool historico_gravar(HISTORICO* hist, FILE* fp){
    if (hist != NULL && fp != NULL){

        int tam = historico_tamanho(hist);
        fwrite(&tam, sizeof(int), 1, fp);

        if (!historico_vazio(hist)) {

            HISTORICO* aux1 = historico_criar();
            for (NO* no = hist->topo; no != NULL; no = no->anterior){
                historico_inserir(aux1, no->procedimento);
            }

            for (NO* no = aux1->topo; no != NULL; no = no->anterior){
                size_t tam_procedimento = strlen(no->procedimento) + 1;
                fwrite(&tam_procedimento, sizeof(size_t), 1, fp);
                fwrite(no->procedimento, sizeof(char),tam_procedimento, fp);
            }

            historico_apagar(&aux1);
        }

        return true;
    }

    return false;
}

HISTORICO* historico_ler(FILE* fp){
    if (fp != NULL){
        HISTORICO* hist = historico_criar();
        if (hist == NULL)
            return NULL;

        int tam = 0;
        fread(&tam, sizeof(int), 1, fp);

        for (int i = 0; i < tam; i++){
            size_t tam_procedimento = 0;
            fread(&tam_procedimento, sizeof(size_t), 1, fp);

            char* procedimento = (char*) malloc(tam_procedimento);
            if (procedimento == NULL){
                historico_apagar(hist);
                return NULL;
            }

            fread(procedimento, sizeof(char), tam_procedimento, fp);
            historico_inserir(hist, procedimento);

            free(procedimento); procedimento = NULL;
        }

        return hist;

    }

    return NULL;
}