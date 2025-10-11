# Projeto 1: Simulação de Pronto Socorro (SUS)
### SCC0202 - Algoritmos e Estruturas de Dados I

Este repositório contém a implementação do primeiro projeto da disciplina de **Algoritmos e Estruturas de Dados I**, ministrada pelos professores **Rudinei** e **JB** no **ICMC/USP**.

O objetivo do projeto é simular o fluxo de atendimento de um pronto socorro, utilizando Tipos Abstratos de Dados (TADs) fundamentais como **Listas**, **Filas** e **Pilhas** na linguagem **C**.

---

## 👥 Equipe
* **Enzo Trulenque Evangelista** - NUSP: 15819219  
* **João Pedro Neves** - NUSP: 14713404  
* **Juan Pablo Tomba** - NUSP: 15638548  

---

## 📜 Sobre o Projeto

O sistema gerencia o fluxo de pacientes em uma unidade de emergência. As principais funcionalidades são:

1. **Registro Geral de Pacientes** – Um registro persistente de todos os pacientes que já passaram pelo hospital.  
2. **Fila de Espera** – Pacientes que dão entrada na emergência são colocados em uma fila de espera (FIFO - First In, First Out).  
3. **Histórico de Tratamento** – Cada paciente possui um histórico de procedimentos médicos, organizado como uma pilha (LIFO - Last In, First Out).  
4. **Persistência de Dados** – As informações são salvas em arquivos binários ao final da execução e recarregadas ao iniciar o sistema.

---

## 🧱 Estruturas de Dados Utilizadas

### `Lista de Pacientes`
* **Propósito:** Manter o registro completo de todos os pacientes.  
* **Funcionalidades:** Inserir, apagar, buscar e listar pacientes.

### `Fila de Espera (Triagem)`
* **Propósito:** Gerenciar a ordem de atendimento dos pacientes.  
* **Critério:** FIFO (First In, First Out).  
* **Funcionalidades:** Enfileirar, desenfileirar, verificar se está cheia/vazia.

### `Histórico Médico`
* **Propósito:** Armazenar os procedimentos realizados em um paciente.  
* **Critério:** LIFO (Last In, First Out).  
* **Funcionalidades:** Empilhar, desempilhar, consultar topo, verificar se está cheia/vazia.

---

## ✨ Funcionalidades do Sistema

O programa é operado via **menu no terminal**, com as seguintes opções:

1. Registrar paciente  
2. Dar alta ao paciente  
3. Adicionar procedimento ao histórico  
4. Desfazer procedimento do histórico  
5. Chamar paciente para atendimento  
6. Mostrar fila de espera  
7. Mostrar histórico do paciente  
8. Sair (salvando os dados em disco)

---

## ⚙️ Como Compilar e Executar

Este projeto utiliza um **Makefile** que detecta automaticamente o sistema operacional (Windows ou Unix-like) e ajusta os comandos e extensões de arquivo.

### 🔧 Requisitos
Antes de compilar, verifique se você possui:
- **GCC** (compilador C) instalado  
- **Make** disponível no terminal  
- (Opcional) Pasta `data/` criada no diretório do projeto para os arquivos binários.

---

### 💻 No Windows (PowerShell ou CMD)

1. Compile o programa:
   Make All
2. Execute:
   Make Run
3. Ao finalizar:
   Make Clean
   
## 💾 Persistência de Dados

Ao encerrar o programa, os seguintes arquivos binários são atualizados automaticamente:

data/lista_itens.bin – Lista de pacientes

data/fila_itens.bin – Fila de espera

Na próxima execução, esses arquivos são carregados para restaurar o estado anterior do sistema.

## 🧠 Observações

Caso os arquivos .bin não existam, eles serão criados automaticamente.

O sistema salva os dados apenas ao encerrar a execução pelo menu principal.

O código foi desenvolvido e testado em ambientes Windows 11 e Ubuntu 24.04.

## 🧩 Exemplo de Execução

Abaixo está um exemplo de execução do sistema no terminal:

-----------------------------------------
     Sistema de Pronto Socorro (SUS)
-----------------------------------------
1. Registrar paciente
2. Dar alta ao paciente
3. Adicionar procedimento ao histórico
4. Desfazer procedimento do histórico
5. Chamar paciente para atendimento
6. Mostrar fila de espera
7. Mostrar histórico do paciente
8. Sair
-----------------------------------------
Escolha uma opção: 1

Digite o CPF: 12345678900

Digite o nome do paciente: João Silva

Paciente cadastrado e adicionado à fila de espera!

-----------------------------------------
1. Registrar paciente
2. Dar alta ao paciente
3. Adicionar procedimento ao histórico
4. Desfazer procedimento do histórico
5. Chamar paciente para atendimento
6. Mostrar fila de espera
7. Mostrar histórico do paciente
8. Sair
-----------------------------------------
Escolha uma opção: 6

Fila de espera:
1º - João Silva

-----------------------------------------
Escolha uma opção: 8
Salvando dados em disco...
Encerrando o sistema. Até logo!

## 🧾 Licença

Este projeto foi desenvolvido para fins educacionais na disciplina SCC0202 - Algoritmos e Estruturas de Dados I.
Todos os direitos reservados aos autores e ao ICMC/USP.

## 🤖 Link do Repositório:

https://github.com/joaopneves1570/Pronto-Socorro-Hospitalar

