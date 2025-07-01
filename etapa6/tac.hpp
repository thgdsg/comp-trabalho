// TAC - hpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#ifndef TAC_HPP
#define TAC_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "symbols.hpp"
#include "ast.hpp"
#define MAX_SONS 4

using namespace std;

enum TACTypes{
    TAC_SYMBOL,
    TAC_VAR_ATTR, TAC_VEC_ATTR, TAC_FUN_ATTR,
    TAC_VAR_LIST, TAC_PARAM_LIST, TAC_EXPR_LIST, TAC_PRINT_LIST,
    TAC_CMD_IF, TAC_CMD_IFELSE, TAC_CMD_WHILE, TAC_CMD_DOWHILE,
    TAC_CMD_ASSIGN, TAC_CMD_VEC_ASSIGN, TAC_CMD_READ, TAC_CMD_PRINT, TAC_CMD_RETURN,
    TAC_VEC, TAC_FUNCALL,
    TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV,
    TAC_LESS, TAC_LEQ, TAC_GREATER, TAC_GEQ,
    TAC_EQUAL, TAC_NEQUAL,
    TAC_AND, TAC_OR,
    TAC_NOT,
    TAC_LABEL,
    TAC_JUMP_FALSE, TAC_JUMP_TRUE,
    TAC_FUNC_START, TAC_FUNC_END
};

struct TAC{
    int tipo;
    SYMBOL* resultado;
    SYMBOL* op1;
    SYMBOL* op2;
    TAC* prev;
    TAC* next;

    public:
        TAC(int t, SYMBOL* res = nullptr, SYMBOL* o1 = nullptr, SYMBOL* o2 = nullptr, TAC* p = nullptr, TAC* n = nullptr)
            : tipo(t), resultado(res), op1(o1), op2(o2), prev(p), next(n) {}
};

void tacPrintSingle(TAC* tac);
void tacPrintBackwards(TAC* tac);
void tacPrintForwards(TAC* tac);
TAC* tacJoin(TAC* a, TAC* b);
TAC* GenerateCode(AST* node);
TAC* tacCreate(int type, SYMBOL* res, SYMBOL* op1, SYMBOL* op2);

TAC* makeBinaryOp(int type, AST* node, TAC* code[]);
TAC* makeIfThen(TAC* code[]);
TAC* makeIfThenElse(TAC* code[]);
TAC* makeWhile(TAC* code[]);
TAC* makeDoWhile(TAC* code[]);
TAC* makeFunction(TAC* code[]);
#endif