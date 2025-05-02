// Árvore de Sintaxe abstrata - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "ast.hpp"
#include "symbols.hpp"
#include <stdio.h>

string ASTTypeNames[] = {
    "AST_UNKNOWN",
    "AST_SYMBOL",
    "AST_ATTR",
    "AST_DEF",
    "AST_ARRAY",
    "AST_ADD", "AST_SUB", "AST_MUL", "AST_DIV",
    "AST_LESS", "AST_LEQ", "AST_GREATER", "AST_GEQ",
    "AST_EQUAL", "AST_NEQUAL",
    "AST_AND", "AST_OR",
    "AST_NOT",
};

void astPrint(AST* nodo){
    if (nodo==0) return;
    printf("AST: %s\n", ASTTypeNames[nodo->tipo].c_str());
    if (nodo->simbolo!=0) {
        printf("Simbolo: %s\n", nodo->simbolo->text.c_str());
    }
    for (int i=0; i<nodo->filho.size(); i++){
        astPrint(nodo->filho[i]);
    }
}