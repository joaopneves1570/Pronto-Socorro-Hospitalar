CFLAGS = -Wall -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -lm

# --- Bloco de Detecção de Sistema Operacional ---
# Verifica se a variável de ambiente OS é "Windows_NT", o que indica um sistema Windows
ifeq ($(OS),Windows_NT)
    RM = del /F /Q
    TARGET = exe.exe
    FILES = data\fila_itens.bin data\lista_itens.bin
# Caso contrário, assume um sistema tipo Unix (Linux, macOS)
else
    RM = rm -f
    TARGET = exe
endif
# --- Fim do Bloco ---

# O target 'all' agora usa a variável TARGET para o nome do arquivo de saída
all:
	@gcc $(CFLAGS) main.c src/IO.c src/fila.c src/lista.c src/paciente.c src/senha.c -I src/include -o $(TARGET) -lm

# O target 'run' também usa a variável TARGET
run:
	@./$(TARGET)

# O target 'clean' usa a variável RM para o comando de remoção
clean:
	@$(RM) $(TARGET) $(FILES)
