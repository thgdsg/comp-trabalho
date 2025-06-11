// TAC - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "tac.hpp"

using namespace std;

TAC* tacCreate(int type, SYMBOL* res, SYMBOL* op1, SYMBOL* op2){
    TAC* newtac = new TAC(type,res,op1,op2,NULL,NULL);
    return newtac;
}

void tacPrintSingle(TAC* tac){
    if (!tac) return;

    fprintf(stderr, "TAC: (%d,%s,%s,%s)\n", tac->tipo, tac->resultado ? tac->resultado->text.c_str() : "null",
           tac->op1 ? tac->op1->text.c_str() : "null",
           tac->op2 ? tac->op2->text.c_str() : "null");
    
}

void tacPrintBackwards(TAC* tac){
    if (!tac) return;

    for(; tac; tac = tac->prev) {
        tacPrintSingle(tac);
    }
}

TAC* tacJoin(TAC* a, TAC* b){
    if (!a) return b;
    if (!b) return a;

    TAC* lastA = a;
    while (lastA->next) {
        lastA = lastA->next;
    }
    lastA->next = b;
    b->prev = lastA;

    return a;
}

TAC* GenerateCode(AST* node){
    int i = 0;
    TAC* result = nullptr;
    TAC* code[MAX_SONS] = {0,0,0,0};
    if (!node) return nullptr;
    for (auto child : node->filho) {
        code[i++] = GenerateCode(child);
    }

    switch (node->tipo){
        case AST_SYMBOL:
            result = new TAC(TAC_SYMBOL, node->simbolo);
            break;
        /*case AST_DEF:
            result = new TAC(TAC_DEF, node->simbolo);
            break;
        case AST_VAR_ATTR:
            result = new TAC(TAC_VAR_ATTR, node->simbolo, code[0]->resultado);
            break;
        case AST_VEC_ATTR:
            result = new TAC(TAC_VEC_ATTR, node->simbolo, code[0]->resultado, code[1]->resultado);
            break;
        case AST_FUN_ATTR:
            result = new TAC(TAC_FUN_ATTR, node->simbolo, code[0]->resultado);
            break;
        case AST_VAR_LIST:
            result = new TAC(TAC_VAR_LIST, node->simbolo);
            break;
        case AST_PARAM_LIST:
            result = new TAC(TAC_PARAM_LIST, node->simbolo);
            break;
        case AST_EXPR_LIST:
            result = new TAC(TAC_EXPR_LIST, node->simbolo);
            break;
        case AST_PRINT_LIST:
            result = new TAC(TAC_PRINT_LIST, node->simbolo);
            break;
        case AST_CMD_LIST:
            result = new TAC(TAC_CMD_LIST, node->simbolo);
            break;
        case AST_CMD_IF:
            result = new TAC(TAC_CMD_IF, nullptr, code[0]->resultado, code[1]->resultado);
            break;
        case AST_CMD_IFELSE:
            result = new TAC(TAC_CMD_IFELSE, nullptr, code[0]->resultado, code[1]->resultado);
            break;
        case AST_CMD_WHILE:
            result = new TAC(TAC_CMD_WHILE, nullptr, code[0]->resultado, code[1]->resultado);
            break;
        case AST_CMD_DOWHILE:
            result = new TAC(TAC_CMD_DOWHILE, nullptr, code[0]->resultado, code[1]->resultado);
            break;
        case AST_CMD_ASSIGN:
            result = new TAC(TAC_CMD_ASSIGN, code[0]->resultado, code[1]->op1);
            break;
        case AST_CMD_VEC_ASSIGN:
            result = new TAC(TAC_CMD_VEC_ASSIGN, code[0]->resultado, code[1]->op1, code[2]->op1);
            break;
        case AST_CMD_READ:
            result = new TAC(TAC_CMD_READ, node->simbolo);
            break;
        case AST_CMD_PRINT:
            result = new TAC(TAC_CMD_PRINT, node->simbolo, code[0]->resultado);
            break;
        case AST_CMD_RETURN:
            result = new TAC(TAC_CMD_RETURN, code[0]->resultado);
            break;
        case AST_BLOCKCMD:  
            result = new TAC(TAC_BLOCKCMD, node->simbolo);
            break;
        case AST_VEC:
            result = new TAC(TAC_VEC, node->simbolo, code[0]->resultado);
            break;
        case AST_FUNCALL:
            result = new TAC(TAC_FUNCALL, node->simbolo, code[0]->resultado);
            for (int j = 1; j < i; ++j) {
                result->op2 = code[j]->resultado;
            }
            break;
        case AST_ADD:
            result = new TAC(TAC_ADD, code[0]->resultado, code[1]->resultado);
            break;
        case AST_SUB:
            result = new TAC(TAC_SUB, code[0]->resultado, code[1]->resultado);
            break;
        case AST_MUL:
            result = new TAC(TAC_MUL, code[0]->resultado, code[1]->resultado);
            break;
        case AST_DIV:
            result = new TAC(TAC_DIV, code[0]->resultado, code[1]->resultado);
            break;
        case AST_LESS:
            result = new TAC(TAC_LESS, code[0]->resultado, code[1]->resultado);
            break;
        case AST_LEQ:
            result = new TAC(TAC_LEQ, code[0]->resultado, code[1]->resultado);
            break;
        case AST_GREATER:
            result = new TAC(TAC_GREATER, code[0]->resultado, code[1]->resultado);
            break;
        case AST_GEQ:
            result = new TAC(TAC_GEQ, code[0]->resultado, code[1]->resultado);
            break;
        case AST_EQUAL:
            result = new TAC(TAC_EQUAL, code[0]->resultado, code[1]->resultado);
            break;
        case AST_NEQUAL:
            result = new TAC(TAC_NEQUAL, code[0]->resultado, code[1]->resultado);
            break;
        case AST_AND:
            result = new TAC(TAC_AND, code[0]->resultado, code[1]->resultado);
            break;
        case AST_OR:
            result = new TAC(TAC_OR, code[0]->resultado, code[1]->resultado);
            break;
        case AST_NOT:
            result = new TAC(TAC_NOT, code[0]->resultado);
            break;*/
        default:
            fprintf(stderr, "Passei pelo tipo %d de AST.\n", node->tipo);
            //fprintf(stderr, "Erro: tipo de AST desconhecido %d\n", node->tipo);
            //exit(EXIT_FAILURE);
    }
    return result;
}