// Árvore de Sintaxe abstrata - hpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#ifndef AST_HPP
#define AST_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "symbols.hpp"

enum ASTTypes{
    AST_UNKNOWN,
    AST_SYMBOL,
    AST_ATTR,
    AST_DEF,
    AST_ARRAY,
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

void astPrint(AST* nodo);

#endif