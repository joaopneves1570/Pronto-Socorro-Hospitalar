#include "../include/IO.h"
#include "../include/paciente.h"
#include "../include/lista.h"
#include "../include/fila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CAMINHO_LISTA "data/lista_itens.bin"
#define CAMINHO_FILA "data/fila_itens.bin"

void gravar_cpf(PACIENTE* paciente, FILE* fp){
    const char* cpf = paciente_obter_cpf(paciente);
    size_t tam_cpf = strlen(cpf) + 1;
    fwrite(&tam_cpf, sizeof(size_t), 1, fp);
    fwrite(cpf, sizeof(char), tam_cpf, fp);
}

void gravar_nome(PACIENTE* paciente, FILE* fp){
    const char* nome = paciente_obter_nome(paciente);
    size_t tam_nome = strlen(nome) + 1;
    fwrite(&tam_nome, sizeof(size_t), 1, fp);
    fwrite(nome, sizeof(char), tam_nome, fp);
}

void gravar_historico(PACIENTE* paciente, FILE* fp){
    HISTORICO* hist = paciente_obter_historico(paciente);
    size_t tam_hist = (size_t) historico_tamanho(hist);
    
    fwrite(&tam_hist, sizeof(size_t), 1, fp);
    
    if (tam_hist > 0) {
        char** buffer_procedimentos = malloc(tam_hist * sizeof(char*));
        if (buffer_procedimentos == NULL) return;

        for (int i = 0; i < tam_hist; i++){
            buffer_procedimentos[i] = historico_remover(hist);
        }
        for (int i = tam_hist - 1; i >= 0; i--){
            if (buffer_procedimentos[i] != NULL) {
                size_t tam_procedimento = strlen(buffer_procedimentos[i]) + 1;
                fwrite(&tam_procedimento, sizeof(size_t), 1, fp);
                fwrite(buffer_procedimentos[i], sizeof(char), tam_procedimento, fp);
                free(buffer_procedimentos[i]); 
            }
        }
        free(buffer_procedimentos); 
    }
}

bool SAVE(LISTA **lista, FILA **fila) {
    if (lista == NULL || *lista == NULL || fila == NULL || *fila == NULL) return false;

    FILE *fp_fila = fopen(CAMINHO_FILA, "wb");
    if (!fp_fila) return false;
    
    int tamanho_original_fila = fila_tamanho(*fila);
    for (int i = 0; i < tamanho_original_fila; i++) {
        PACIENTE* pac_na_fila = fila_remover(*fila);
        if (pac_na_fila) {
            gravar_cpf(pac_na_fila, fp_fila);
            fila_inserir(*fila, pac_na_fila);
        }
    }
    fclose(fp_fila);

    FILE *fp_lista = fopen(CAMINHO_LISTA, "wb");
    if (!fp_lista) return false;

    while (!lista_vazia(*lista)) {
        PACIENTE* pac_atual = lista_remover_inicio(*lista);
        gravar_cpf(pac_atual, fp_lista);
        gravar_nome(pac_atual, fp_lista);
        gravar_historico(pac_atual, fp_lista);
        paciente_apagar(&pac_atual);
    }
    fclose(fp_lista);
    
    lista_apagar(lista);
    fila_apagar(fila);

    return true;
}

char* ler_string(FILE* fp) {
    size_t tamanho;
    if (fread(&tamanho, sizeof(size_t), 1, fp) != 1) {
        return NULL;
    }

    char* str = malloc(tamanho);
    if (str) {
        if (fread(str, sizeof(char), tamanho, fp) != tamanho) {
            free(str);
            return NULL;
        }
    }
    return str;
}

void ler_historico(PACIENTE* paciente, FILE* fp){
    size_t num_procedimentos;
    if (fread(&num_procedimentos, sizeof(size_t), 1, fp) != 1) return;
    
    if (num_procedimentos > 0) {
        HISTORICO* historico = paciente_obter_historico(paciente);
        for (size_t i = 0; i < num_procedimentos; i++) {
            char* procedimento = ler_string(fp);
            if (procedimento) {
                historico_inserir(historico, procedimento);
                free(procedimento);
            }
        }
    }
}

bool LOAD(LISTA **lista, FILA **fila) {
    if (lista == NULL || *lista == NULL || fila == NULL || *fila == NULL) return false;

    FILE* fp_lista = fopen(CAMINHO_LISTA, "rb");
    if (fp_lista) {
        char* cpf;
        while ((cpf = ler_string(fp_lista)) != NULL) {
            char* nome = ler_string(fp_lista);
            
            PACIENTE* pac_atual = paciente_criar(nome, cpf);
            ler_historico(pac_atual, fp_lista);
            lista_inserir(*lista, pac_atual);

            free(cpf);
            free(nome);
        }
        fclose(fp_lista);
    }

    FILE* fp_fila = fopen(CAMINHO_FILA, "rb");
    if (fp_fila) {
        char* cpf_fila;
        while ((cpf_fila = ler_string(fp_fila)) != NULL) {
            PACIENTE* paciente_encontrado = lista_buscar(*lista, cpf_fila);
            if (paciente_encontrado) {
                fila_inserir(*fila, paciente_encontrado);
            }
            free(cpf_fila);
        }
        fclose(fp_fila);
    }

    return true;
}