make -s clean
make -s
# compila o código fonte e transforma o arquivo "teste" em assembly
./etapa6 teste6.txt saida.s
gcc -c saida.s
gcc saida.o -o saida
# executa o código ASM compilado
./saida
make -s clean
rm saida