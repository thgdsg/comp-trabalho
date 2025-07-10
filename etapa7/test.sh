make -s clean
make -s
# compila o código fonte e transforma o arquivo "teste" em assembly
./etapa7 teste_todos.txt saida.s
gcc -c saida.s
gcc saida.o -o saida
# executa o código ASM compilado
./saida
make -s clean
rm saida