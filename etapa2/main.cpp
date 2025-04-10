// main
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "symbols.hpp"

extern FILE* yyin;
extern int isRunning();
extern int getLineNumber();
int yylex();
int yyparse();
// extern int yydebug; // pra debug se precisar
extern int running;
extern int line_count;

int main(int argc, char** argv){
    if (argc<2){
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    if((yyin=fopen(argv[1], "r"))==0){
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
    // yydebug = 1; // Ativa o modo debug do Bison
    yyparse();
    fprintf(stderr,"Parsing finished\n");
    fprintf(stderr,"File has %d lines\n", getLineNumber());
    symbolPrintTable();
}