all:
	@gcc main.c src/IO.c src/fila.c src/historico.c src/lista.c src/paciente.c -I src/include -o exe

run:
	@./exe

clean:
	@rm -f exe
