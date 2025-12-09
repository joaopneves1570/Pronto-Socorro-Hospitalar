#include "../include/IO.h"
#include "../include/paciente.h"
#include "../include/lista.h"
#include "../include/fila.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool SAVE(LISTA **lista, FILA **fila)
{
    if (!lista || !(*lista) || !fila || !*(fila))
        return false;

    PACIENTE *paciente;
    char *str_paciente;
    int tamanho_str_paciente;
    int prioridade_capturada; // Variável para guardar a prioridade

    // --- Salvando Fila (Com Prioridade) ---

    FILE *fp_fila = fopen("data/fila_itens.bin", "wb");
    if (!fp_fila) return false;

    // Usa a nova função que recupera a prioridade junto com o paciente
    while ((paciente = fila_remover_com_prioridade(*fila, &prioridade_capturada)))
    {
        str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

        if (str_paciente) 
        {
            // 1. Salva a PRIORIDADE (0-4)
            fwrite(&prioridade_capturada, sizeof(int), 1, fp_fila);
            
            // 2. Salva o TAMANHO da string
            fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_fila);
            
            // 3. Salva os DADOS do paciente
            fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_fila);
            
            free(str_paciente);
        }
    }
    fclose(fp_fila);
    fila_apagar(fila);

    // --- Salvando Lista (Igual ao anterior) ---

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

bool LOAD(LISTA **lista, FILA **fila)
{
    if (!lista || !(*lista) || !fila || !(*fila)) return false;

    // --- Carregando Lista ---

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

    // --- Carregando Fila (Com Prioridade) ---

    FILE *fp_fila = fopen("data/fila_itens.bin", "rb");
    if (fp_fila != NULL)
    {
        int prioridade_lida;
        int tamanho_str_paciente;

        // Note a ordem: Lê Prioridade -> Lê Tamanho -> Lê String
        while (fread(&prioridade_lida, sizeof(int), 1, fp_fila) == 1)
        {
            // Agora lê o tamanho
            if (fread(&tamanho_str_paciente, sizeof(int), 1, fp_fila) == 1)
            {
                char *buffer = calloc(tamanho_str_paciente + 1, sizeof(char));
                if(buffer)
                {
                    fread(buffer, sizeof(char), tamanho_str_paciente, fp_fila);
                    
                    char cpf_temp[12];
                    strncpy(cpf_temp, buffer, 11);
                    cpf_temp[11] = '\0';
                    
                    PACIENTE *paciente_encontrado = lista_buscar(*lista, cpf_temp);
                    
                    if (paciente_encontrado)
                    {
                        // INSERE NA FILA USANDO A PRIORIDADE CORRETA LIDA DO ARQUIVO
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