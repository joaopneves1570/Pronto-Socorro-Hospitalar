#include "../include/IO.h"
#include "../include/paciente.h"
#include "../include/lista.h"
#include "../include/fila.h" 

/**
 * @brief Salva o estado atual da lista de pacientes e da fila de espera em arquivos binários.
 * @details Esta função é destrutiva. Ela serializa cada paciente da fila e da lista,
 * escrevendo-os em "data/fila_itens.bin" e "data/lista_itens.bin", respectivamente.
 * Durante o processo, a função esvazia e apaga completamente as estruturas de dados
 * originais (fila e lista) para liberar a memória.
 * @param lista Ponteiro para o ponteiro da lista de pacientes.
 * @param fila Ponteiro para o ponteiro da fila de espera.
 * @return true se ambas as estruturas foram salvas com sucesso, false em caso de erro.
 */
bool SAVE(LISTA **lista, FILA **fila)
{
    if (!lista || !(*lista) || !fila || !*(fila))
        return false;

    PACIENTE *paciente;
    char *str_paciente;
    int tamanho_str_paciente;

    // --- Salvando os itens da fila ---

    // Abre o arquivo para escrita binária. O diretório "data" deve existir.
    FILE *fp_fila = fopen("data/fila_itens.bin", "wb");
    if (!fp_fila)
        return false;

    // Remove cada paciente da fila, um por um, até que ela esteja vazia.
    while ((paciente = fila_remover(*fila)))
    {
        str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

        if (str_paciente) // Garante que a serialização ocorreu com sucesso
        {
            // Escreve o tamanho da string, seguido pela string de dados do paciente.
            fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_fila);
            fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_fila);
            free(str_paciente);
            str_paciente = NULL;
        }
    }
    fclose(fp_fila); fp_fila = NULL;

    // Como a fila foi esvaziada, apaga a estrutura para liberar memória.
    fila_apagar(fila);

    // --- Salvando os itens da lista ---

    FILE *fp_lista = fopen("data/lista_itens.bin", "wb");
    if (!fp_lista)
        return false;

    // Remove cada paciente da lista, um por um, até que ela esteja vazia.
    while ((paciente = lista_remover_inicio(*lista)))
    {
        str_paciente = paciente_para_string(paciente, &tamanho_str_paciente);

        if (str_paciente)
        {
            // Escreve os dados serializados no arquivo.
            fwrite(&tamanho_str_paciente, sizeof(int), 1, fp_lista);
            fwrite(str_paciente, sizeof(char), tamanho_str_paciente, fp_lista);

            // Apaga o paciente removido para liberar memória.
            paciente_apagar(&paciente);
            free(str_paciente);
            str_paciente = NULL;
        }
    }
    fclose(fp_lista); fp_lista = NULL;
    
    // Apaga a estrutura da lista vazia
    lista_apagar(lista);

    return true;
}

/**
 * @brief Carrega os dados dos pacientes da lista e da fila a partir de arquivos binários.
 * @details Lê os arquivos "data/lista_itens.bin" and "data/fila_itens.bin", se existirem.
 * Para cada entrada, desserializa os dados e recria as estruturas de PACIENTE, inserindo-as
 * na lista e na fila, respectivamente, repovoando o estado do programa.
 * @param lista Ponteiro para o ponteiro da lista a ser preenchida.
 * @param fila Ponteiro para o ponteiro da fila a ser preenchida.
 * @return true sempre (a função não falha mesmo que os arquivos não existam), 
 * ou false se os ponteiros de entrada forem nulos.
 */
bool LOAD(LISTA **lista, FILA **fila)
{
    if (!lista || !(*lista) || !fila || !(*fila))
        return false;

    // --- Carregando os itens da lista ---

    FILE *fp_lista = fopen("data/lista_itens.bin", "rb");
    if (fp_lista != NULL)
    {
        int tamanho_str_paciente;
        // Lê o tamanho do próximo registro. O loop continua enquanto a leitura for bem sucedida.
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

    // --- Carregando os itens da fila ---

    FILE *fp_fila = fopen("data/fila_itens.bin", "rb");
    if (fp_fila != NULL)
    {
        int tamanho_str_paciente;
        while (fread(&tamanho_str_paciente, sizeof(int), 1, fp_fila) == 1)
        {
            char *buffer = calloc(tamanho_str_paciente + 1, sizeof(char));
            if(buffer)
            {
                fread(buffer, sizeof(char), tamanho_str_paciente, fp_fila);
                
                // Busca o paciente já criado na lista para não duplicar dados.
                char cpf_temp[12];
                strncpy(cpf_temp, buffer, 11);
                cpf_temp[11] = '\0';
                
                PACIENTE *paciente_encontrado = lista_buscar(*lista, cpf_temp);
                if (paciente_encontrado)
                {
                    fila_inserir(*fila, paciente_encontrado);
                }
                free(buffer);
            }
        }
        fclose(fp_fila);
    }

    return true;
}