/**
 * @file lista.c
 * @brief Implementação de uma estrutura de dados para gerenciar pacientes.
 * @details Embora a interface sugira uma "Lista", a implementação interna utiliza uma 
 * Árvore AVL (Árvore Binária de Busca Balanceada) ordenadada pelo CPF.
 * Isso garante complexidade O(log n) para busca, inserção e remoção.
 */

#include "../include/lista.h"

/**
 * @struct no_
 * @brief Estrutura que representa um nó da Árvore AVL.
 */
typedef struct no_ NO;
struct no_{
    NO* esq;        /**< Ponteiro para o filho à esquerda. */
    NO* dir;        /**< Ponteiro para o filho à direita. */
    PACIENTE* pac;  /**< Ponteiro para o dado (Paciente). */
    int altura;     /**< Altura do nó para cálculo do fator de balanceamento. */
};

/**
 * @struct lista_
 * @brief Estrutura wrapper que contém a raiz da árvore.
 */
struct lista_{
    NO* raiz;       /**< Nó raiz da árvore AVL. */
};

/**
 * @brief Definição do tipo de função de callback para percorrer a estrutura.
 */
typedef void (*AcaoPaciente)(PACIENTE* p, void* contexto);

/**
 * @brief Cria e inicializa uma nova lista (árvore) vazia.
 * @return LISTA* Ponteiro para a estrutura alocada ou NULL se falhar.
 */
LISTA* lista_criar(void){
    LISTA* lista = (LISTA*)malloc(sizeof(LISTA));
    if (lista != NULL){
        lista->raiz = NULL;
    }
    return lista;
}

/**
 * @brief Função auxiliar para criar um novo nó contendo um paciente.
 * @param p Ponteiro para o paciente a ser armazenado.
 * @return NO* Ponteiro para o novo nó ou NULL se falhar a alocação.
 */
NO* lista_cria_no(PACIENTE* p){
    NO* novo = (NO*)malloc(sizeof(NO));
    if (novo != NULL){
        novo->altura = 0;
        novo->dir = NULL;
        novo->esq = NULL;
        novo->pac = p;
    }
    return novo;
}

/**
 * @brief Retorna a altura de um nó de forma segura (trata NULL).
 * @param raiz Ponteiro para o nó.
 * @return int Altura do nó ou -1 se o nó for NULL.
 */
int lista_altura_no(NO* raiz){
    if (raiz != NULL){
        return raiz->altura;
    }
    return -1;
}

// --- Rotações ---

/**
 * @brief Realiza uma rotação simples à direita para balancear a AVL.
 * @details Usada quando o fator de balanceamento indica desequilíbrio à esquerda-esquerda.
 * @param a O nó que está desbalanceado.
 * @return NO* A nova raiz da subárvore após a rotação.
 */
NO* rodar_direita(NO* a){
    
    NO* b = a->esq;
    a->esq = b->dir;
    b->dir = a;

    a->altura = max(lista_altura_no(a->esq), lista_altura_no(a->dir)) + 1;
    b->altura = max(lista_altura_no(b->esq), lista_altura_no(b->dir)) + 1;

    return b;
}

/**
 * @brief Realiza uma rotação simples à esquerda para balancear a AVL.
 * @details Usada quando o fator de balanceamento indica desequilíbrio à direita-direita.
 * @param a O nó que está desbalanceado.
 * @return NO* A nova raiz da subárvore após a rotação.
 */
NO* rodar_esquerda(NO* a){
    NO* b = a->dir;
    a->dir = b->esq;
    b->esq = a;

    a->altura = max(lista_altura_no(a->esq), lista_altura_no(a->dir)) + 1;
    b->altura = max(lista_altura_no(b->esq), lista_altura_no(b->dir)) + 1;

    return b;
}

/**
 * @brief Realiza uma rotação dupla: Direita depois Esquerda.
 * @details Usada para corrigir o caso Direita-Esquerda (Joelho à direita).
 * @param a O nó desbalanceado.
 * @return NO* A nova raiz da subárvore.
 */
NO* rodar_direita_esquerda(NO* a){
    NO* b = a->dir;
    b = rodar_direita(b);
    return rodar_esquerda(a);
}

/**
 * @brief Realiza uma rotação dupla: Esquerda depois Direita.
 * @details Usada para corrigir o caso Esquerda-Direita (Joelho à esquerda).
 * @param a O nó desbalanceado.
 * @return NO* A nova raiz da subárvore.
 */
NO* rodar_esquerda_direita(NO* a){
    NO* b = a->esq;
    b = rodar_esquerda(b);
    return rodar_direita(a);
}

// --- Inserção ---

/**
 * @brief Função recursiva interna para inserir um nó e rebalancear a árvore.
 * @param raiz Raiz da subárvore atual.
 * @param p Paciente a ser inserido.
 * @return NO* Nova raiz da subárvore (pode mudar devido a rotações).
 */
NO* lista_inserir_no(NO* raiz, PACIENTE* p){
    if (raiz == NULL)
        raiz = lista_cria_no(p);
    

    int cmp = strcmp(paciente_obter_cpf(p), paciente_obter_cpf(raiz->pac));

    if (cmp < 0){
        raiz->esq = lista_inserir_no(raiz->esq, p);
    } else if (cmp > 0){
        raiz->dir = lista_inserir_no(raiz->dir, p);
    }
    // Se cmp == 0, CPF é igual, não insere duplicado (ou atualiza, dependendo da lógica desejada)

    raiz->altura = max(lista_altura_no(raiz->esq), lista_altura_no(raiz->dir)) + 1;
    int FB = lista_altura_no(raiz->esq) - lista_altura_no(raiz->dir);

    if (FB == -2){
        if (lista_altura_no(raiz->dir->esq) - lista_altura_no(raiz->dir->dir) <= 0)
            raiz = rodar_esquerda(raiz);
        else
            raiz = rodar_direita_esquerda(raiz);
    } else if (FB == 2){
        if (lista_altura_no(raiz->esq->esq) - lista_altura_no(raiz->esq->dir) >= 0)
            raiz = rodar_direita(raiz);
        else
            raiz = rodar_esquerda_direita(raiz);
    }

    return raiz;
}

/**
 * @brief Insere um paciente na estrutura.
 * @param l Ponteiro para a lista (árvore).
 * @param p Ponteiro para o paciente.
 * @return true Se inseriu com sucesso.
 * @return false Se a lista não existir ou erro de alocação.
 */
bool lista_inserir(LISTA* l, PACIENTE* p){
    if (l != NULL){
        // Verifica se a raiz mudou (pode ter rotacionado ou sido criada)
        l->raiz = lista_inserir_no(l->raiz, p);
        return (l->raiz != NULL); 
    }
    return false;
}

// --- Remoção ---

/**
 * @brief Função auxiliar para remoção de nó com 2 filhos.
 * @details Busca o maior elemento da subárvore esquerda (antecessor) para substituir o nó removido.
 * @param atual Nó sendo percorrido na busca do máximo.
 * @param raiz Nó original que será removido (onde os dados serão copiados).
 * @param ant Pai do nó 'atual'.
 * @param pac Ponteiro para armazenar o paciente removido (backup).
 */
void troca_max_esq(NO* atual, NO* raiz, NO* ant, PACIENTE** pac){
    if (atual->dir != NULL){
        troca_max_esq(atual->dir, raiz, atual, pac);
        return;
    }

    if (ant == raiz)
        ant->esq = atual->esq;
    else
        ant->dir = atual->esq;
    
    if (pac != NULL)
        *pac = raiz->pac;

    raiz->pac = atual->pac;

    free(atual);
    atual = NULL;
}

/**
 * @brief Remove um nó da árvore AVL recursivamente baseado na chave (CPF).
 * @param raiz Raiz da subárvore atual.
 * @param chave String contendo o CPF a ser removido.
 * @param pac Retorno por referência do ponteiro do paciente removido.
 * @return NO* Nova raiz da subárvore após remoção e balanceamento.
 */
NO* lista_remover_no(NO* raiz, char* chave, PACIENTE** pac){
    NO* p;
    
    if (raiz == NULL) return NULL;

    int cmp = strcmp(chave, paciente_obter_cpf(raiz->pac));

    if (cmp == 0){
        if (raiz->esq == NULL || raiz->dir == NULL){
            p = raiz;

            if (pac != NULL) {
                *pac = p->pac;
            }

            if (raiz->esq == NULL)
                raiz = raiz->dir;
            else
                raiz = raiz->esq;
            
            free(p);
            p = NULL;
        } else {
            troca_max_esq(raiz->esq, raiz, raiz, pac);
        }
    } else if (cmp < 0){
        raiz->esq = lista_remover_no(raiz->esq, chave, pac);
    } else if (cmp > 0){
        raiz->dir = lista_remover_no(raiz->dir, chave, pac);
    }

    if (raiz != NULL){
        raiz->altura = max(lista_altura_no(raiz->esq), lista_altura_no(raiz->dir)) + 1;
        int FB = lista_altura_no(raiz->esq) - lista_altura_no(raiz->dir);

        if (FB == -2){
            if (lista_altura_no(raiz->dir->esq) - lista_altura_no(raiz->dir->dir) <= 0)
                raiz = rodar_esquerda(raiz);
            else
                raiz = rodar_direita_esquerda(raiz);
        } else if (FB == 2){
            if (lista_altura_no(raiz->esq->esq) - lista_altura_no(raiz->esq->dir) >= 0)
                raiz = rodar_direita(raiz);
            else
                raiz = rodar_esquerda_direita(raiz);
        }
    }

    return raiz;
}

/**
 * @brief Remove um paciente específico da lista.
 * @param l Ponteiro para a lista.
 * @param p Ponteiro para o paciente (usado para obter o CPF).
 * @return PACIENTE* O ponteiro do paciente removido (para que o chamador possa dar free se necessário).
 */
PACIENTE* lista_remover(LISTA* l, PACIENTE* p){
    if (l != NULL && !(lista_vazia(l))){
        PACIENTE* paciente_recuperado = NULL;
        // Passa a string CPF direto
        l->raiz = lista_remover_no(l->raiz, paciente_obter_cpf(p), &paciente_recuperado);
        return paciente_recuperado;
    }
    return NULL;
}

/**
 * @brief Remove o "último" paciente da lista (aquele com o maior CPF).
 * @note Esta função percorre até a extrema direita, mas chama a remoção 
 * a partir da raiz para garantir o rebalanceamento de toda a árvore.
 * @param l Ponteiro para a lista.
 * @return PACIENTE* Ponteiro do paciente removido.
 */
PACIENTE* lista_remover_ultimo(LISTA* l){
    if (l != NULL && !(lista_vazia(l))){
        PACIENTE* paciente_recuperado = NULL;
        NO* atual = l->raiz;
        
        while (atual->dir != NULL){
            atual = atual->dir;
        }
        
        char cpf_ultimo[16]; // Buffer seguro para guardar o CPF
        strcpy(cpf_ultimo, paciente_obter_cpf(atual->pac));

        l->raiz = lista_remover_no(l->raiz, cpf_ultimo, &paciente_recuperado);
        
        return paciente_recuperado;
    }
    return NULL;
}

// --- Utilidades ---

/**
 * @brief Verifica se a lista está vazia.
 * @param l Ponteiro para a lista.
 * @return true Se a raiz for NULL.
 * @return false Caso contrário.
 */
bool lista_vazia(LISTA* l){
    if (l != NULL){
        return l->raiz == NULL;
    }
    return true;
}

/**
 * @brief Verifica se a lista está cheia.
 * @note Como é implementada com alocação dinâmica, teoricamente nunca está cheia.
 * @param l Ponteiro para a lista.
 * @return false Sempre retorna falso.
 */
bool lista_cheia(LISTA* l){
    if (l != NULL){
        NO* novo = (NO*)malloc(sizeof(NO));
        if (novo != NULL){
            free(novo);
            return false;
        }
        return true;
    }

    return false;
}

/**
 * @brief Busca recursiva interna de um nó pelo CPF.
 * @param raiz Raiz da subárvore.
 * @param cpf String do CPF a buscar.
 * @param p Retorno por referência do paciente encontrado.
 * @return NO* O nó contendo o paciente ou NULL se não encontrado.
 */
NO* lista_buscar_no(NO* raiz, char* cpf, PACIENTE** p){
    if (raiz == NULL)
        return NULL;
    
    int cmp = strcmp(cpf, paciente_obter_cpf(raiz->pac));
    
    if (cmp == 0){
        *p = raiz->pac;
        return raiz;
    }
    else if (cmp < 0)
        return lista_buscar_no(raiz->esq, cpf, p);
    else
        return lista_buscar_no(raiz->dir, cpf, p);
}

/**
 * @brief Busca um paciente na lista pelo CPF.
 * @param l Ponteiro para a lista.
 * @param cpf String do CPF.
 * @return PACIENTE* Ponteiro para o paciente encontrado ou NULL.
 */
PACIENTE* lista_buscar(LISTA* l, char* cpf){
    if (l != NULL){
        PACIENTE* paciente_buscado = NULL;
        // Passa a string CPF direto
        lista_buscar_no(l->raiz, cpf, &paciente_buscado);
        return paciente_buscado;
    }
    return NULL;
}

// --- Impressão e Limpeza ---

/**
 * @brief Percorre a árvore em ordem (In-Order) e executa uma ação.
 * @note Resulta nos dados processados em ordem crescente de CPF.
 * @param raiz Raiz da subárvore.
 * @param acao Ponteiro de função a ser executada em cada nó.
 * @param contexto Parâmetro extra opcional para a função de callback.
 */
void lista_em_ordem(NO* raiz, AcaoPaciente acao, void* contexto){
    if (raiz != NULL){
        lista_em_ordem(raiz->esq, acao, contexto);
        acao(raiz->pac, contexto);
        lista_em_ordem(raiz->dir, acao, contexto);
    }
}

/**
 * @brief Percorre a árvore em pré-ordem (Pre-Order).
 * @param raiz Raiz da subárvore.
 * @param acao Ponteiro de função a ser executada em cada nó.
 * @param contexto Parâmetro extra opcional.
 */
void lista_pre_ordem(NO* raiz, AcaoPaciente acao, void* contexto){
    if (raiz != NULL){
        acao(raiz->pac, contexto); 
        lista_pre_ordem(raiz->esq, acao, contexto); 
        lista_pre_ordem(raiz->dir, acao, contexto);
    }
}

/**
 * @brief Função de callback auxiliar para imprimir os dados de um paciente.
 * @param p O paciente atual.
 * @param contexto Não utilizado neste caso (NULL).
 */
void acao_imprimir_lista(PACIENTE* p, void* contexto){
    char* estado;
    if (paciente_esta_na_fila(p))
        estado = "NA FILA DE ATENDIMENTO";
    else
        estado = "FORA DA FILA DE ATENDIMENTO";
    
    printf("NOME: %s\nCPF: %s\nESTADO: %s\n\n", paciente_obter_nome(p), paciente_obter_cpf(p), estado);  
}

/**
 * @brief Exibe todos os pacientes da lista no terminal em ordem de CPF.
 * @param l Ponteiro para a lista.
 */
void lista_mostrar(LISTA* l){
    if (l != NULL){
        printf("Lista de Pacientes (em ordem crescente de CPF):\n");
        lista_em_ordem(l->raiz, acao_imprimir_lista, NULL);    
    }
}

/**
 * @brief Função auxiliar para apagar os nós da árvore (Pós-ordem).
 * @param raiz Raiz da subárvore a ser apagada.
 */
void lista_apagar_aux(NO* raiz){
    if (raiz != NULL){
        lista_apagar_aux(raiz->esq);
        lista_apagar_aux(raiz->dir);
        paciente_apagar(&raiz->pac); 
        free(raiz);
    }
}

/**
 * @brief Apaga a lista inteira, liberando a memória dos nós e dos pacientes.
 * @param l Endereço do ponteiro da lista (LISTA**).
 */
void lista_apagar(LISTA** l){
    if (*l != NULL){
        lista_apagar_aux((*l)->raiz);
        free(*l);
        *l = NULL;
    }
}