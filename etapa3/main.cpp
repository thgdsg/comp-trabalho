// main
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <stdio.h>
#include <stdlib.h>
#include "symbols.hpp"
#include "ast.hpp"

extern FILE* yyin;
extern int isRunning();
extern int getLineNumber();
extern AST* raiz;
int yylex();
int yyparse();
//extern int yydebug; // pra debug se precisar
extern int running;
extern int line_count;

int main(int argc, char** argv){
    if (argc<3){
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
    }
    if((yyin=fopen(argv[1], "r"))==0){
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
//    yydebug = 1; // Ativa o modo debug do Bison
    yyparse();
    fprintf(stderr,"Parsing finished\n");
    fprintf(stderr,"File has %d lines\n", getLineNumber());
    symbolPrintTable();
    // gera o .txt usando o nome passado em argv[2]
    astGenerateToFile(raiz, argv[2]);
    exit(0);
}