// Árvore de Sintaxe abstrata - hpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#ifndef AST_HPP
#define AST_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstdint>
#include "symbols.hpp"

enum ASTTypes{
    AST_UNKNOWN,
    AST_SYMBOL,
    AST_DEF,
    AST_VAR_ATTR, AST_VEC_ATTR, AST_FUN_ATTR,
    AST_VAR_LIST, AST_PARAM_LIST, AST_EXPR_LIST, AST_PRINT_LIST, AST_CMD_LIST,
    AST_CMD_IF, AST_CMD_IFELSE, AST_CMD_WHILE, AST_CMD_DOWHILE,
    AST_CMD_ASSIGN, AST_CMD_VEC_ASSIGN, AST_CMD_READ, AST_CMD_PRINT, AST_CMD_RETURN, 
    AST_BLOCKCMD,
    AST_VEC, AST_FUNCALL,
    AST_ADD, AST_SUB, AST_MUL, AST_DIV,
    AST_LESS, AST_LEQ, AST_GREATER, AST_GEQ,
    AST_EQUAL, AST_NEQUAL,
    AST_AND, AST_OR,
    AST_NOT
};

struct AST{
    int tipo;
    vector<AST*> filho;
    SYMBOL* simbolo;
    public: AST (int t, vector<AST*> v, SYMBOL* s=0): tipo(t), filho(v), simbolo(s) {};
};

char* reverseInt(const char* s);
char* reverseRealNumber(const char* s);
void astPrint(AST* nodo, int nivel=0);
void astGenerate(AST* nodo, FILE* out, int indent = 0);
void astGenerateToFile(AST* raiz, const char* filename);

#endif