# Projeto 1: Simulação de Pronto Socorro (SUS)
### SCC0202 - Algoritmos e Estruturas de Dados I

Este repositório contém a implementação do primeiro projeto da disciplina de Algoritmos e Estruturas de Dados I, ministrada pelos professores Rudinei e JB no ICMC/USP.

O objetivo do projeto é simular o fluxo de atendimento de um pronto socorro, utilizando Tipos Abstratos de Dados (TADs) fundamentais como **Listas**, **Filas** e **Pilhas** na linguagem C/C++.

---

## 👥 Equipe
* **Enzo Trulenque Evangelista** - NUSP: 
* **João Pedro Neves** - NUSP: 14713404
* **Juan Pablo Tomba** - NUSP: 15638548

## 📜 Sobre o Projeto

O sistema gerencia o fluxo de pacientes em uma unidade de emergência. As principais funcionalidades são:
1.  **Registro Geral de Pacientes**: Um registro persistente de todos os pacientes que já passaram pelo hospital.
2.  **Fila de Espera**: Pacientes que dão entrada na emergência são colocados em uma fila de espera, seguindo a ordem de chegada (FIFO - First-In, First-Out).
3.  **Histórico de Tratamento**: Cada paciente possui um histórico de procedimentos médicos, que funciona como uma pilha (LIFO - Last-In, First-Out), permitindo que o último procedimento seja desfeito primeiro.
4.  **Persistência de Dados**: Todas as informações (lista de pacientes, fila de espera e históricos) são salvas em disco ao final da execução e recarregadas ao iniciar o sistema.

## 🧱 Estruturas de Dados Utilizadas

O núcleo do sistema é construído sobre os seguintes Tipos Abstratos de Dados:

* ### `Lista de Pacientes`
    * **Propósito**: Manter um registro de todos os pacientes, mesmo após a alta.
    * **Implementação**: 
    * **Funcionalidades**: Inserir, apagar, buscar e listar todos os pacientes.

* ### `Fila de Espera (Triagem)`
    * **Propósito**: Gerenciar a ordem de atendimento dos pacientes na sala de espera.
    * **Critério**: Primeiro a Chegar, Primeiro a Ser Atendido (FIFO).
    * **Implementação**: 
    * **Funcionalidades**: Enfileirar (inserir), desenfileirar (remover), verificar se está cheia ou vazia.

* ### `Histórico Médico`
    * **Propósito**: Armazenar a sequência de procedimentos médicos de um paciente.
    * **Critério**: Último a Entrar, Primeiro a Sair (LIFO). Ideal para desfazer a última ação.
    * **Implementação**: 
    * **Funcionalidades**: Empilhar (inserir), desempilhar (retirar), consultar topo, verificar se está cheia ou vazia.

## ✨ Funcionalidades do Sistema

A interface do sistema é operada através de um menu de console com as seguintes opções:

1.  **Registrar paciente**: Cadastra um novo paciente no sistema (caso não exista) e o insere na fila de espera.
2.  **Dar alta ao paciente**: Remove um paciente da fila de espera (simulando a liberação do leito de emergência). O registro do paciente permanece no sistema.
3.  **Adicionar procedimento ao histórico**: Adiciona um novo procedimento médico ao histórico de um paciente específico.
4.  **Desfazer procedimento do histórico**: Remove o último procedimento adicionado ao histórico de um paciente.
5.  **Chamar paciente para atendimento**: Retira o próximo paciente da fila de espera para ser atendido.
6.  **Mostrar fila de espera**: Exibe todos os pacientes atualmente na fila e sua ordem.
7.  **Mostrar histórico do paciente**: Exibe o histórico de procedimentos de um paciente específico.
8.  **Sair**: Salva todos os dados em disco e encerra o programa.
