make -s clean
make -s
./etapa6 teste4.txt saida.s
gcc -c saida.s
gcc saida.o -o saida
make -s clean