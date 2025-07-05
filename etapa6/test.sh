make -s clean
make -s
./etapa6 teste6.txt saida.s
gcc -c saida.s
gcc saida.o -o saida
./saida
make -s clean