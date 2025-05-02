// Árvore de Sintaxe abstrata - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "ast.hpp"
#include "symbols.hpp"
#include <stdio.h>

string ASTTypeNames[] = {
    "AST_UNKNOWN",
    "AST_SYMBOL",
    "AST_DEF",
    "AST_VAR_ATTR", "AST_VEC_ATTR", "AST_FUN_ATTR",
    "AST_VAR_LIST", "AST_PARAM_LIST", "AST_EXPR_LIST", "AST_PRINT_LIST", "AST_CMD_LIST",
    "AST_CMD_IF", "AST_CMD_IFELSE", "AST_CMD_WHILE", "AST_CMD_DOWHILE",
    "AST_CMD_ASSIGN", "AST_CMD_VEC_ASSIGN", "AST_CMD_READ", "AST_CMD_PRINT", "AST_CMD_RETURN", 
    "AST_BLOCKCMD",
    "AST_VEC", "AST_FUNCALL",
    "AST_ADD", "AST_SUB", "AST_MUL", "AST_DIV",
    "AST_LESS", "AST_LEQ", "AST_GREATER", "AST_GEQ",
    "AST_EQUAL", "AST_NEQUAL",
    "AST_AND", "AST_OR",
    "AST_NOT",
};

void astPrint(AST* nodo, int nivel){
    for(int i=0; i<nivel; i++){
        printf("  ");
    }
    if (nodo==0) return;
    printf("AST: %s\n", ASTTypeNames[nodo->tipo].c_str());
    if (nodo->simbolo!=0) {
        for(int i=0; i<nivel; i++){
            printf("  ");
        }
        printf("Simbolo: %s, Tipo: %d\n", nodo->simbolo->text.c_str(), nodo->simbolo->type);
    }
    for (uint32_t i=0; i<nodo->filho.size(); i++){
        astPrint(nodo->filho[i], nivel+1);
    }
}