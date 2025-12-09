/**
 * @file IO.c
 * @brief Implementação da interface textual do sistema de Pronto Socorro.
 *
 * Este módulo contém funções de entrada/saída, menus, validações de CPF,
 * e o loop principal da aplicação (função main).
 *
 * Ele interage com os TADs:
 * - LISTA  (cadastro de pacientes)
 * - FILA   (fila de prioridade)
 * - HISTORICO (registro de atendimentos)
 * - PACIENTE (estrutura principal do paciente)
 */

#include "include/IO.h"
#include "include/fila.h"
#include "include/historico.h"
#include "include/lista.h"
#include "include/paciente.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// Inclusões para limpar a tela de forma portável (Windows/Linux)
#ifdef _WIN32
#include <windows.h>
#define LIMPAR_TELA "cls"
#else
#include <unistd.h>
#define LIMPAR_TELA "clear"
#endif

// Definições de cores e estilos ANSI
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_STYLE_BOLD    "\x1b[1m"

/**
 * @brief Limpa o terminal de forma compatível com Windows e Linux.
 */
void limpar_tela() { system(LIMPAR_TELA); }

/**
 * @brief Pausa a execução até que o usuário aperte Enter.
 */
void pausar_para_continuar()
{
    printf("\n" ANSI_STYLE_BOLD "Pressione Enter para continuar..." ANSI_COLOR_RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

/**
 * @brief Imprime um cabeçalho destacado com o título da seção atual.
 * @param titulo Texto exibido no cabeçalho.
 */
void imprimir_cabecalho(const char *titulo)
{
    limpar_tela();
    printf(ANSI_STYLE_BOLD ANSI_COLOR_CYAN "--- %s ---\n\n" ANSI_COLOR_RESET, titulo);
}

/**
 * @brief Exibe o menu principal do sistema.
 *
 * Inclui as opções previstas no projeto + uma opção extra
 * para manipulação manual do histórico.
 */
void exibir_menu_principal()
{
    limpar_tela();
    printf(ANSI_STYLE_BOLD ANSI_COLOR_CYAN "--- Pronto Socorro SUS (V2) ---\n\n" ANSI_COLOR_RESET);
    printf("1. Registrar Entrada (Cadastro/Fila)\n");
    printf("2. Remover Paciente (Do Sistema)\n");
    printf("3. Listar Todos os Pacientes\n");
    printf("4. Buscar Paciente por ID (CPF)\n");
    printf("5. Mostrar Fila de Espera\n");
    printf("6. Dar Alta ao Paciente (Atendimento)\n");
    printf("7. Sair\n");
    printf("-------------------------------\n");
    printf("8. [Extra] Gerenciar Histórico (Manual)\n"); 
    printf("\nEscolha uma opção: ");
}

/**
 * @brief Enumeração das opções do menu principal.
 */
typedef enum
{
    REGISTRAR_ENTRADA = 1,
    REMOVER_PACIENTE = 2,
    LISTAR_PACIENTES = 3,
    BUSCAR_PACIENTE = 4,
    MOSTRAR_FILA = 5,
    DAR_ALTA = 6,
    SAIR = 7,
    EXTRA_HISTORICO = 8
} Opcao;

/**
 * @brief Lê a escolha do usuário no menu principal.
 * @return Uma opção válida da enumeração Opcao.
 */
Opcao escolher_opcao()
{
    int opcao;
    do {
        scanf("%d", &opcao);
        getchar(); // Limpar buffer
        if (opcao < 1 || opcao > 8) printf("Opção inválida! Tente novamente: ");
    } while (opcao < 1 || opcao > 8);
    return (Opcao)opcao;
}

/**
 * @brief Remove caracteres não numéricos do CPF.
 * @param cpf String contendo o CPF digitado.
 */
void formatar_cpf(char cpf[])
{
    int tamanho = 0;
    for (int i = 0; cpf[i] != '\0'; i++) {
        if (isdigit(cpf[i])) {
            cpf[tamanho] = cpf[i];
            tamanho++;
        }
    }
    cpf[tamanho] = '\0';
}

/**
 * @brief Verifica se um CPF tem comprimento válido (11 dígitos).
 * @param cpf String contendo o CPF.
 * @return true se válido, false caso contrário.
 */
bool eh_cpf_valido(char cpf[])
{
    formatar_cpf(cpf);
    if (strlen(cpf) != 11) {
        printf("CPF inválido! (Tamanho incorreto)\n");
        return false;
    }
    return true; 
}

/**
 * @brief Lê um CPF do usuário e valida o formato.
 * @return String alocada dinamicamente com o CPF, ou NULL se inválido.
 */
char *cpf_ler()
{
    char *cpf = calloc(16, sizeof(char));
    if (!cpf) return NULL;

    printf("Digite o CPF (apenas números): ");
    fgets(cpf, 16, stdin);
    cpf[strcspn(cpf, "\n")] = '\0'; 

    if (!eh_cpf_valido(cpf)) {
        free(cpf);
        return NULL;
    }
    return cpf;
}

/**
 * @brief Exibe o menu de classificação de risco e lê a prioridade.
 * @return Índice da prioridade (0 a 4).
 */
int ler_prioridade_interface()
{
    int p;
    printf("\n--- Classificação de Risco ---\n");
    printf("1. Emergência (Risco de morte imediato)\n");
    printf("2. Muito Urgente (Grave, risco de evoluir)\n");
    printf("3. Urgente (Grave moderado)\n");
    printf("4. Pouco Urgente\n");
    printf("5. Não Urgência\n");
    printf("Selecione a prioridade (1-5): ");
    
    do {
        scanf("%d", &p);
        getchar(); // Limpar buffer
        if (p < 1 || p > 5) printf("Opção inválida. Digite entre 1 e 5: ");
    } while (p < 1 || p > 5);

    return p - 1;
}

/**
 * @brief Função principal do sistema de pronto-socorro.
 *
 * Controla:
 * - Cadastro e remoção de pacientes
 * - Inserção e remoção da fila
 * - Registros em histórico
 * - Persistência dos dados (LOAD/SAVE)
 *
 * @return 0 ao finalizar a execução.
 */
int main()
{
    // Inicialização das estruturas
    LISTA *lista = lista_criar(); 
    FILA *fila = fila_criar();    

    // Carregar dados do disco
    if (!LOAD(&lista, &fila)) {
        printf(ANSI_COLOR_YELLOW "[AVISO] Base de dados nova ou erro ao carregar. Iniciando vazio.\n" ANSI_COLOR_RESET);
        pausar_para_continuar();
    } else {
        printf(ANSI_COLOR_GREEN "[SUCESSO] Dados carregados do disco.\n" ANSI_COLOR_RESET);
        #ifdef _WIN32
        Sleep(1000);
        #else
        sleep(1);
        #endif
    }

    Opcao opcao;

    do {
        exibir_menu_principal();
        opcao = escolher_opcao();

        switch (opcao)
        {
        /**
         * @brief Registrar entrada e triagem de um paciente.
         */
        case REGISTRAR_ENTRADA:
        {
            imprimir_cabecalho("Registrar Entrada / Triagem");
            char *cpf = cpf_ler();
            if (cpf) {
                PACIENTE *pac = lista_buscar(lista, cpf);

                // Cadastro caso não exista
                if (pac == NULL) {
                    char nome[256];
                    printf("Paciente não cadastrado. Digite o Nome Completo: ");
                    fgets(nome, 256, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    pac = paciente_criar(nome, cpf);
                    lista_inserir(lista, pac);
                    printf(ANSI_COLOR_GREEN "[NOVO] Paciente cadastrado no sistema.\n" ANSI_COLOR_RESET);
                } else {
                    printf(ANSI_COLOR_CYAN "[ENCONTRADO] Paciente já possui cadastro: %s\n" ANSI_COLOR_RESET, paciente_obter_nome(pac));
                }

                // Verifica duplicidade na fila
                if (paciente_esta_na_fila(pac)) {
                     printf(ANSI_COLOR_YELLOW "[ALERTA] Paciente já está na fila de espera.\n" ANSI_COLOR_RESET);
                } else {
                    int prioridade = ler_prioridade_interface();
                    if (fila_inserir(fila, pac, prioridade)) {
                        printf(ANSI_COLOR_GREEN "[SUCESSO] Paciente encaminhado para fila.\n" ANSI_COLOR_RESET);
                    } else {
                        printf(ANSI_COLOR_RED "[ERRO] Falha ao inserir na fila.\n" ANSI_COLOR_RESET);
                    }
                }
                
                free(cpf);
            }
            break;
        }

        /**
         * @brief Remove um paciente definitivamente do sistema.
         */
        case REMOVER_PACIENTE:
        {
            imprimir_cabecalho("Remover Paciente do Sistema");
            char *cpf = cpf_ler();
            if (cpf) {
                PACIENTE *pac = lista_buscar(lista, cpf);
                if (pac) {
                    if (paciente_esta_na_fila(pac)) {
                        printf(ANSI_COLOR_YELLOW "[ALERTA] Paciente está na fila! Não é seguro remover.\n" ANSI_COLOR_RESET);
                    } else {
                        lista_remover(lista, pac);
                        printf(ANSI_COLOR_GREEN "[SUCESSO] Paciente removido.\n" ANSI_COLOR_RESET);
                    }
                } else {
                    printf(ANSI_COLOR_RED "[ERRO] Paciente não encontrado.\n" ANSI_COLOR_RESET);
                }
                free(cpf);
            }
            break;
        }

        /**
         * @brief Exibe a lista completa de pacientes cadastrados.
         */
        case LISTAR_PACIENTES:
        {
            imprimir_cabecalho("Lista Geral de Pacientes");
            lista_mostrar(lista); 
            break;
        }

        /**
         * @brief Busca um paciente pelo CPF e exibe seus dados.
         */
        case BUSCAR_PACIENTE:
        {
            imprimir_cabecalho("Buscar Paciente");
            char *cpf = cpf_ler();
            if (cpf) {
                PACIENTE *pac = lista_buscar(lista, cpf);
                if (pac) {
                    printf("Nome: " ANSI_STYLE_BOLD "%s" ANSI_COLOR_RESET "\n", paciente_obter_nome(pac));
                    printf("CPF: %s\n", paciente_obter_cpf(pac));
                    
                    if (paciente_esta_na_fila(pac)) 
                        printf("Status: " ANSI_COLOR_YELLOW "Aguardando Atendimento\n" ANSI_COLOR_RESET);
                    else 
                        printf("Status: " ANSI_COLOR_GREEN "Sem pendências atuais\n" ANSI_COLOR_RESET);

                    printf("\nDeseja visualizar o histórico médico? (s/n): ");
                    char resp = getchar();
                    if (resp == 's' || resp == 'S')
                        historico_imprimir(paciente_obter_historico(pac));

                } else {
                    printf(ANSI_COLOR_RED "[ERRO] Paciente não encontrado.\n" ANSI_COLOR_RESET);
                }
                free(cpf);
            }
            break;
        }

        /**
         * @brief Imprime a fila de espera com prioridades.
         */
        case MOSTRAR_FILA:
        {
            imprimir_cabecalho("Fila de Espera (Prioridades)");
            fila_imprimir(fila);
            break;
        }

        /**
         * @brief Dá alta ao próximo paciente da fila.
         */
        case DAR_ALTA:
        {
            imprimir_cabecalho("Atendimento / Alta");
            
            if (fila_vazia(fila)) {
                printf(ANSI_COLOR_YELLOW "Não há pacientes aguardando atendimento.\n" ANSI_COLOR_RESET);
            } else {
                PACIENTE *pac = fila_remover(fila);
                
                if (pac) {
                    printf(ANSI_STYLE_BOLD "ATENDENDO PACIENTE: %s\n" ANSI_COLOR_RESET, paciente_obter_nome(pac));
                    printf("CPF: %s\n\n", paciente_obter_cpf(pac));

                    printf("Deseja registrar o procedimento? (s/n): ");
                    char op;
                    scanf(" %c", &op);
                    getchar(); 

                    if (op == 's' || op == 'S') {
                        char proc[256];
                        printf("Descreva o procedimento: ");
                        fgets(proc, 256, stdin);
                        proc[strcspn(proc, "\n")] = '\0';
                        
                        historico_inserir(paciente_obter_historico(pac), proc);
                        printf(ANSI_COLOR_GREEN "[REGISTRADO] Procedimento salvo.\n" ANSI_COLOR_RESET);
                    }

                    printf(ANSI_COLOR_GREEN "\n[ALTA] Paciente liberado com sucesso.\n" ANSI_COLOR_RESET);
                }
            }
            break;
        }

        /**
         * @brief Manipulação manual do histórico médico.
         */
        case EXTRA_HISTORICO:
        {
            imprimir_cabecalho("Gestão Manual de Histórico");
            char *cpf = cpf_ler();
            if (cpf) {
                PACIENTE *pac = lista_buscar(lista, cpf);
                if (pac) {
                    printf("Paciente: " ANSI_STYLE_BOLD "%s" ANSI_COLOR_RESET "\n", paciente_obter_nome(pac));
                    printf("1. Adicionar Procedimento\n");
                    printf("2. Desfazer Último\n");
                    printf("3. Mostrar Histórico Completo\n");
                    printf("Escolha: ");
                    
                    int h_op;
                    scanf("%d", &h_op);
                    getchar();

                    HISTORICO *hist = paciente_obter_historico(pac);
                    
                    if (h_op == 1) {
                        char proc[256];
                        printf("Procedimento: ");
                        fgets(proc, 256, stdin);
                        proc[strcspn(proc, "\n")] = '\0';
                        historico_inserir(hist, proc);
                        printf(ANSI_COLOR_GREEN "Adicionado.\n" ANSI_COLOR_RESET);
                    } 
                    else if (h_op == 2) {
                        char *removido = historico_remover(hist);
                        if (removido) {
                            printf(ANSI_COLOR_YELLOW "Desfeito: %s\n" ANSI_COLOR_RESET, removido);
                            free(removido);
                        } else {
                            printf(ANSI_COLOR_RED "Histórico vazio.\n" ANSI_COLOR_RESET);
                        }
                    } 
                    else if (h_op == 3) {
                        printf("\n--- Visualização de Histórico ---\n");
                        historico_imprimir(hist);
                    } 
                    else {
                        printf(ANSI_COLOR_RED "Opção inválida.\n" ANSI_COLOR_RESET);
                    }
                } else {
                    printf(ANSI_COLOR_RED "Paciente não encontrado.\n" ANSI_COLOR_RESET);
                }
                free(cpf);
            }
            break;
        }

        case SAIR:
            imprimir_cabecalho("Encerrando Sistema");
            printf("Salvando dados em disco...\n");
            break;
        }

        if (opcao != SAIR) pausar_para_continuar();

    } while (opcao != SAIR);

    SAVE(&lista, &fila);
    lista_apagar(&lista);
    fila_apagar(&fila);

    printf(ANSI_COLOR_GREEN "Sistema encerrado com segurança.\n" ANSI_COLOR_RESET);
    return 0;
}
