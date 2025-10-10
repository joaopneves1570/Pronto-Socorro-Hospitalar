#include "include/IO.h"
#include "include/fila.h"
#include "include/historico.h"
#include "include/lista.h"
#include "include/paciente.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Op??es do menu principal
typedef enum
{
    REGISTRAR_PACIENTE = 1,
    REGISTRAR_OBITO_DE_PACIENTE = 2,
    ADICIONAR_PROCEDIMENTO_AO_HISTORICO_MEDICO = 3,
    DESFAZER_PROCEDIMENTO_DO_HISTORICO_MEDICO = 4,
    CHAMAR_PACIENTE_PARA_ATENDIMENTO = 5,
    MOSTRAR_LISTA_DE_ESPERA = 6,
    MOSTRAR_HISTORICO_DO_PACIENTE = 7,
    SAIR = 8,
} Opcao;

/**
 * @brief L? o input do usu?rio, repetindo e avisando caso for inv?lido
 *
 * @return a enum correspondente ? op??o escolhida
 */
Opcao escolher_opcao()
{
    Opcao opcao_escolhida;
    int opcao_int;

    // Repete a tentativa de escolha at? uma escolha v?lida
    do
    {
        scanf("%d", &opcao_int);
        getchar();

        printf("\n");

        // Confere se a op??o escolhida pelo usu?rio est? no intervalo v?lido
        if (opcao_int < 1 || opcao_int > 8)
            printf("Op??o inv?lida! Escolha uma das op??es acima (1-8): ");
    } while (opcao_int < 1 || opcao_int > 8);

    opcao_escolhida = (Opcao)opcao_int;
    return opcao_escolhida;
}

/**
 * @brief Altera a string formatando-a como xxxxxxxxxxxx + \0 + lixo
 *
 * @param cpf chave ?nica do paciente
 */
void formatar_cpf(char cpf[])
{
    int tamanho = 0;

    // Itera sobre os caracteres deslocando cada d?gito para esquerda de acordo com o n?mero de caracteres estranhos encontrados
    for (int i = 0; cpf[i] != '\0'; i++)
    {
        printf("%c", cpf[i]);
        if (isdigit(cpf[i]))
        {
            cpf[tamanho] = cpf[i];
            tamanho++;
        }
    }

    cpf[tamanho] = '\0';
}

/**
 * @brief formata o CPF e confere se ele ? v?lido, avisando o usu?rio caso contr?rio
 *
 * @param cpf chave ?nica do paciente
 * @return true se o CPF ? valido, false caso contr?rio
 */
bool eh_cpf_valido(char cpf[])
{
    formatar_cpf(cpf);

    int digitos_iguais_ao_primeiro = 0;

    // ? inv?lido se h? caracteres estranhos na string
    for (int i = 0; i < 11; i++)
    {
        if (cpf[i] == cpf[0])
            digitos_iguais_ao_primeiro++;
        if (cpf[i] == '\0')
        {
            printf("CPF inv?lido! (Muito curto)");
            return false;
        }
        else if (!isdigit(cpf[i]))
        {
            printf("CPF inv?lido! (Caracteres estranhos)");
            return false;
        }
    }

    if (digitos_iguais_ao_primeiro == 11)
    {
        printf("CPF inv?lido! (Combina??o proibida)");
        return false;
    }

    // Algoritmo da verifica??o de CPF
    int soma = 0, dv[2];

    // Primeiro d?gito verificador
    for (int i = 0; i < 9; i++)
        soma += (cpf[i] - '0') * (10 - i);

    dv[0] = soma % 11 > 1 ? 11 - soma % 11 : 0;

    // Confere se o primeiro d?gito verificador fornecido ? o que deveria ser de acordo com o algoritmo
    if (cpf[9] - '0' != dv[0])
    {
        printf("CPF inv?lido! (Esse CPF n?o existe)");
        return false;
    }

    soma = 0;

    // Segundo d?gito verificador
    for (int i = 0; i < 10; i++)
        soma += (cpf[i] - '0') * (11 - i);

    dv[1] = soma % 11 > 1 ? 11 - soma % 11 : 0;

    // Confere se o segundo d?gito verificador fornecido ? o que deveria ser de acordo com o algoritmo
    if (cpf[10] - '0' != dv[1])
    {
        printf("CPF inv?lido! (Esse CPF n?o existe)");
        return false;
    }

    // Se n?o falhar em nenhum teste, retorna verdadeiro
    return true;
}

/**
 * @brief Pede que o usu?rio digite um CPF, aloca a string dinamicamente para recebe-lo e testa se ? v?lido
 *
 * @return um ponteiro para char de 15 posi??es alocado dinamicamente se v?lido, NULL caso contr?rio
 */
char *cpf_ler()
{
    // Tenta alocar o string
    char *cpf = calloc(16, sizeof(char));

    // Se falhar retorna um ponteiro nulo
    if (cpf == NULL)
        return NULL;

    // Mostra a instru??o TODO:: Tamb?m confere se ? valido, att a descri??o
    printf("Digite o cpf: ");
    fgets(cpf, 16, stdin);
    cpf[14] = '\0';
    printf("\n");

    if (!eh_cpf_valido(cpf))
    {
        free(cpf);
        cpf = NULL;
    }

    // printf("CPF v?lido.\n");
    return cpf;
}

/**
 * @brief Tenta obter um paciente da lista de acordo com um CPF que o cliente digitar?
 *
 * @param lista a lista de pacientes
 * @return o paciente com o CPF fornecido pelo cliente, NULL caso algo d? errado
 */
PACIENTE *paciente_ler(LISTA *lista)
{
    PACIENTE *paciente = NULL;
    char *cpf = cpf_ler();

    if (cpf)
        paciente = lista_buscar(lista, cpf);

    free(cpf);

    return paciente;
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    LISTA *lista = lista_criar();
    FILA *fila = fila_criar();

    printf("passei aqui.\n");
    if (!LOAD(&lista, &fila))
        return -1;

    printf("passei aqui tbm.\n");

    char opcoes[][64] =
        {
            {"1. Registrar paciente"},
            {"2. Registrar ?bito de paciente"},
            {"3. Adicionar procedimento ao hist?rico m?dico"},
            {"4. Desfazer procedimento do hist?rico m?dico"},
            {"5. Chamar paciente para atendimento"},
            {"6. Mostrar fila de espera"},
            {"7. Mostrar hist?rico do paciente"},
            {"8. Sair"},
        };

    Opcao opcao_escolhida;

    do
    {
        printf("=========================================\n");
        for (int i = 0; i < 8; i++)
            printf("%s\n", opcoes[i]);

        printf("\n\nEscolha uma das op??es acima: ");
        opcao_escolhida = escolher_opcao();

        printf("Voc? escolheu: %s\n\n", opcoes[opcao_escolhida - 1]);

        switch (opcao_escolhida)
        {
        case REGISTRAR_PACIENTE:
        {
            char *cpf = cpf_ler();

            if (cpf == NULL)
                break;

            printf("CPF deu certo\n");

            PACIENTE *paciente = lista_buscar(lista, cpf);

            printf("Buscou o paciente\n");

            if (paciente)
                printf("Paciente j? cadastrado.\n");
            else
            {
                char nome[256];

                printf("Digite o nome do paciente: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("\n");

                paciente = paciente_criar(nome, cpf);

                lista_inserir(lista, paciente);
                printf("Paciente cadastrado com sucesso!\n");
            }

            if (fila_inserir(fila, paciente))
                printf("Paciente adicionado ? fila com sucesso!\n");

            free(cpf);

            break;
        }
        case REGISTRAR_OBITO_DE_PACIENTE:
        {
            PACIENTE *paciente = paciente_ler(lista);

            if (paciente)
            {
                if (fila_buscar(fila, paciente_obter_cpf(paciente)))
                {
                    printf("O paciente est? na fila de espera, ainda h? esperan?a!\n");
                    break;
                }
                paciente = lista_remover(lista, paciente);
                paciente_apagar(&paciente);

                printf("Registro do paciente removido de acordo com a LGPDb.\n");
            }
            else
                printf("Paciente n?o encontrado!\n");

            break;
        }
        case ADICIONAR_PROCEDIMENTO_AO_HISTORICO_MEDICO:
        {
            PACIENTE *paciente = paciente_ler(lista);

            if (paciente)
            {
                char procedimento[256];

                printf("Escreva o procedimento: ");
                fgets(procedimento, sizeof(procedimento), stdin);
                procedimento[strcspn(procedimento, "\n")] = '\0';
                printf("\n");

                HISTORICO *historico = paciente_obter_historico(paciente);

                if (historico)
                {
                    if (historico_inserir(historico, procedimento))
                        printf("Procedimento adicionado ao hist?rico do paciente.\n");
                    else
                        printf("O procedimento n?o p?de ser adicionado ao hist?rico do paciente.\n");
                }
                else
                    printf("Falha ao obter o hist?rico do paciente.\n");
            }
            else
                printf("O paciente n?o foi encontrado.\n");

            break;
        }
        case DESFAZER_PROCEDIMENTO_DO_HISTORICO_MEDICO:
        {
            PACIENTE *paciente = paciente_ler(lista);

            if (paciente)
            {
                HISTORICO *historico = paciente_obter_historico(paciente);

                if (historico)
                {
                    if (historico_vazio(historico))
                        printf("O hist?rico do paciente est? vazio.\n");
                    else
                    {
                        char *procedimento = historico_remover(historico);
                        if (procedimento)
                        {
                            printf("O seguinte procedimento retirado do hist?rico do paciente:\n%s\n", procedimento);
                            free(procedimento);
                        }
                        else
                            printf("O ?ltimo procedimento n?o p?de ser removido do hist?rico do paciente.\n");
                    }
                }
                else
                    printf("Falha ao obter o hist?rico do paciente.\n");
            }
            else
                printf("O paciente n?o foi encontrado.\n");

            break;
        }
        case CHAMAR_PACIENTE_PARA_ATENDIMENTO:
        {
            if (fila_vazia(fila))
            {
                printf("A fila est? vazia.\n");
                break;
            }

            PACIENTE *paciente = fila_remover(fila);

            if (paciente)
                printf("O paciente %s ? o pr?ximo a ser atendido.\n", paciente_obter_nome(paciente));
            else
                printf("O paciente n?o foi encontrado.\n");

            break;
        }
        case MOSTRAR_LISTA_DE_ESPERA:
        {
            fila_imprimir(fila);

            break;
        }
        case MOSTRAR_HISTORICO_DO_PACIENTE:
        {
            PACIENTE *paciente = paciente_ler(lista);

            if (paciente)
            {
                HISTORICO *historico = paciente_obter_historico(paciente);

                if (historico)
                    historico_imprimir(historico);
                else
                    printf("Falha ao obter o hist?rico do paciente.\n");
            }
            else
                printf("O paciente n?o foi encontrado.\n");

            break;
        }
        case SAIR:
            break;
        }

        printf("\n");
    } while (opcao_escolhida != SAIR);

    SAVE(&lista, &fila);
    fila_apagar(&fila);
    lista_apagar(&lista);
    
    return 0;
}
