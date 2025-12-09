/**
 * @file IO.c
 * @brief Implementa as rotinas de salvar e carregar os dados persistentes
 *        do sistema, incluindo a lista de pacientes e a fila com prioridade.
 *
 * O sistema utiliza dois arquivos binários:
 *  - data/lista_itens.bin  → Contém todos os pacientes cadastrados
 *  - data/fila_itens.bin   → Contém a fila de espera com prioridade
 *
 * A serialização dos pacientes é feita como STRING, gerada por `paciente_para_string()`.
 * Na fila, cada registro é salvo com PRIORIDADE → TAMANHO → STRING.
 */

#include "../include/IO.h"
#include "../include/paciente.h"
#include "../include/lista.h"
#include "../include/fila.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Salva a LISTA e a FILA em disco, em formato binário.
 *
 * A função percorre:
 * 
 * 1. **Fila:**  
 *    Remove cada paciente (mantendo sua prioridade original), converte para string
 *    e salva no arquivo `data/fila_itens.bin` como:
 *    
 *      - prioridade (int)
 *      - tamanho da string (int)
 *      - string do paciente (bytes)
 *
 * 2. **Lista:**  
 *    Remove cada paciente e salva apenas:
 *    
 *      - tamanho da string (int)
 *      - string do paciente (bytes)
 *
 * Depois, ambas as estruturas são apagadas (resetadas).
 *
 * @param lista Ponteiro para o ponteiro da LISTA principal
 * @param fila  Ponteiro para o ponteiro da FILA principal
 * @return true se salvar tudo com sucesso, false caso contrário
 */
bool SAVE(LISTA **lista, FILA **fila)
{
    if (!lista || !(*lista) || !fila || !*(fila))
        return false;

    PACIENTE *paciente;
    char *str_paciente;
    int tamanho_str_paciente;
    int prioridade_capturada;

    /* --- Salvando Fila (com prioridade) --- */

    FILE *fp_fila = fopen("data/fila_itens.bin", "wb");
    if (!fp_fila) return false;

    /**
     * A função fila_remover_com_prioridade() devolve o paciente
     * e preenche a variavel 'prioridade_capturada'.
     */
    while ((paciente = fila_remover_com_prioridade(*fila, &prioridade_capturada)))
    {
        str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

        if (str_paciente) 
        {
            /* Salva PRIORIDADE */
            fwrite(&prioridade_capturada, sizeof(int), 1, fp_fila);
            /* Salva TAMANHO */
            fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_fila);
            /* Salva STRING */
            fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_fila);
            
            free(str_paciente);
        }
    }
    fclose(fp_fila);
    fila_apagar(fila);

    /* --- Salvando Lista de Pacientes --- */

    FILE *fp_lista = fopen("data/lista_itens.bin", "wb");
    if (!fp_lista) return false;

    while ((paciente = lista_remover_ultimo(*lista)))
    {
        str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

        if (str_paciente)
        {
            fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_lista);
            fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_lista);
            paciente_apagar(&paciente);
            free(str_paciente);
        }
    }
    fclose(fp_lista);
    lista_apagar(lista);

    return true;
}

/**
 * @brief Carrega a LISTA e a FILA a partir dos arquivos binários.
 *
 * O carregamento ocorre na ordem:
 * 
 * 1. **Lista:**  
 *    Lê cada registro, reconstrói o paciente usando `paciente_de_string()`  
 *    e reinsere na LISTA.
 *
 * 2. **Fila:**  
 *    Lê na ordem:
 *      - prioridade
 *      - tamanho da string
 *      - string do paciente
 *
 *    Recupera o CPF dos primeiros 11 bytes, busca o paciente na lista
 *    e reinsere na fila com a prioridade correta.
 *
 * @param lista Ponteiro para o ponteiro da LISTA já criada
 * @param fila  Ponteiro para o ponteiro da FILA já criada
 * @return true se carregou com sucesso, false caso contrário
 */
bool LOAD(LISTA **lista, FILA **fila)
{
    if (!lista || !(*lista) || !fila || !(*fila)) return false;

    /* --- Carregando Lista --- */

    FILE *fp_lista = fopen("data/lista_itens.bin", "rb");
    if (fp_lista != NULL)
    {
        int tamanho_str_paciente;

        while (fread(&tamanho_str_paciente, sizeof(int), 1, fp_lista) == 1)
        {
            char *buffer = calloc(tamanho_str_paciente + 1, sizeof(char));
            if (buffer)
            {
                fread(buffer, sizeof(char), tamanho_str_paciente, fp_lista);
                PACIENTE *paciente = paciente_de_string(buffer);
                lista_inserir(*lista, paciente);
                free(buffer);
            }
        }
        fclose(fp_lista);
    }

    /* --- Carregando Fila (com prioridade) --- */

    FILE *fp_fila = fopen("data/fila_itens.bin", "rb");
    if (fp_fila != NULL)
    {
        int prioridade_lida;
        int tamanho_str_paciente;

        /**
         * Ordem de leitura:
         *  1. prioridade
         *  2. tamanho
         *  3. string
         */
        while (fread(&prioridade_lida, sizeof(int), 1, fp_fila) == 1)
        {
            if (fread(&tamanho_str_paciente, sizeof(int), 1, fp_fila) == 1)
            {
                char *buffer = calloc(tamanho_str_paciente + 1, sizeof(char));
                if(buffer)
                {
                    fread(buffer, sizeof(char), tamanho_str_paciente, fp_fila);

                    /* Extrai CPF dos primeiros 11 caracteres */
                    char cpf_temp[12];
                    strncpy(cpf_temp, buffer, 11);
                    cpf_temp[11] = '\0';

                    /* Busca paciente na lista já carregada */
                    PACIENTE *paciente_encontrado = lista_buscar(*lista, cpf_temp);

                    if (paciente_encontrado)
                    {
                        /* Insere paciente na fila com a prioridade original */
                        fila_inserir(*fila, paciente_encontrado, prioridade_lida);
                    }

                    free(buffer);
                }
            }
        }
        fclose(fp_fila);
    }

    return true;
}
