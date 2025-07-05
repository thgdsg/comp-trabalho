// main
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <stdio.h>
#include <stdlib.h>
#include "symbols.hpp"
#include "ast.hpp"
#include "semantic.hpp"
#include "tac.hpp"
#include "asm.hpp"
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
        exit(1);
    }
    if((yyin=fopen(argv[1], "r"))==0){
        printf("Cannot open file %s\n", argv[1]);
        exit(2);
    }
//    yydebug = 1; // Ativa o modo debug do Bison
    yyparse();
    fprintf(stderr,"Parsing finished\n");
    fprintf(stderr,"File has %d lines\n", getLineNumber());
    //symbolPrintTable();

    // gera o .txt usando o nome passado em argv[2]
    //astGenerateToFile(raiz, argv[2]);
    int errosSemanticos = semanticCheck(raiz);
    
    //symbolPrintTable();
    if (errosSemanticos > 0) {
        fprintf(stderr, "Found %d semantic errors\n", errosSemanticos);
        exit(4);
    }
    TAC* codigo_TAC = GenerateCode(raiz);
    // por padrão, imprime a TAC de cima pra baixo
    // se necessário, pode imprimir de baixo pra cima

    //fprintf(stderr, "Warning: arvore gerada de baixo pra cima. Caso queira gerar ao contrario, altere o final do arquivo main.cpp\n");
    //tacPrintBackwards(codigo_TAC);

    //tacPrintForwards(codigo_TAC);
    //fprintf(stderr, "Warning: arvore gerada de cima pra baixo. Caso queira gerar ao contrario, altere o final do arquivo main.cpp\n");
    
    // gera o .asm usando o nome passado em argv[3]
    FILE* out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "Cannot open output file %s\n", argv[2]);
        exit(3);
    }
    asmGenerate(codigo_TAC, out);
    exit(0);
}