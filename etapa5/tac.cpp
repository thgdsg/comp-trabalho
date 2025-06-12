// TAC - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "tac.hpp"

using namespace std;

string TACTypes[] = {
    "TAC_UNKNOWN",
    "TAC_SYMBOL",
    "TAC_DEF",
    "TAC_VAR_ATTR", "TAC_VEC_ATTR", "TAC_FUN_ATTR",
    "TAC_VAR_LIST", "TAC_PARAM_LIST", "TAC_EXPR_LIST", "TAC_PRINT_LIST", "TAC_CMD_LIST",
    "TAC_CMD_IF", "TAC_CMD_IFELSE", "TAC_CMD_WHILE", "TAC_CMD_DOWHILE",
    "TAC_CMD_ASSIGN", "TAC_CMD_VEC_ASSIGN", "TAC_CMD_READ", "TAC_CMD_PRINT", "TAC_CMD_RETURN",
    "TAC_BLOCKCMD",
    "TAC_VEC", "TAC_FUNCALL",
    "TAC_ADD", "TAC_SUB", "TAC_MUL", "TAC_DIV",
    "TAC_LESS", "TAC_LEQ", "TAC_GREATER", "TAC_GEQ",
    "TAC_EQUAL", "TAC_NEQUAL",
    "TAC_AND", "TAC_OR",
    "TAC_NOT",
    "TAC_LABEL"
};

TAC* tacCreate(int type, SYMBOL* res, SYMBOL* op1, SYMBOL* op2){
    TAC* newtac = new TAC(type,res,op1,op2,NULL,NULL);
    return newtac;
}

void tacPrintSingle(TAC* tac){
    if (!tac) return;
    if (tac->tipo == TAC_SYMBOL) return;

    fprintf(stderr, "TAC: (%s,%s,%s,%s)\n", TACTypes[tac->tipo].c_str(), tac->resultado ? tac->resultado->text.c_str() : "null",
           tac->op1 ? tac->op1->text.c_str() : "null",
           tac->op2 ? tac->op2->text.c_str() : "null");
    
}

void tacPrintBackwards(TAC* tac){
    if (!tac) return;

    for(; tac; tac = tac->prev) {
        tacPrintSingle(tac);
    }
}

TAC* tacJoin(TAC* l1, TAC* l2){
    if (!l1) return l2;
    if (!l2) return l1;

    TAC* point;

    for (point = l2; point->prev !=0; point = point->prev)
    ;

    point->prev = l1;

    return l2;
}

TAC* makeBinaryOp(int type, TAC* code[]){

    return tacJoin(tacJoin(code[0],code[1]), tacCreate(type,symbolMakeTemp(),code[0] ? code[0]->resultado : 0 ,code[1] ? code[1]->resultado : 0));
}

TAC* GenerateCode(AST* node){
    int i = 0;
    TAC* result = nullptr;
    TAC* code[MAX_SONS] = {0,0,0,0};

    if (!node) return nullptr;

    // faz os filhos primeiro, bottom-up
    for (auto child : node->filho) {
        code[i++] = GenerateCode(child);
    }

    switch (node->tipo){
        case AST_SYMBOL:
            result = tacCreate(TAC_SYMBOL,node->simbolo,0,0);
            break;
        case AST_VEC:
            result = tacJoin(code[0], 
            tacCreate(TAC_VEC, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_VAR_ATTR:
            result = tacJoin(code[0], 
            tacCreate(TAC_VAR_ATTR, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_VEC_ATTR:
            result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]),
            tacCreate(TAC_VEC_ATTR, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, code[2] ? code[2]->resultado : 0));
            break;
        case AST_FUN_ATTR:
            if (node->filho.size() == 3) {
                result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]),
                tacCreate(TAC_FUN_ATTR, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            } else {
                result = tacJoin(tacJoin(code[0], code[1]),
                tacCreate(TAC_FUN_ATTR, code[0] ? code[0]->resultado : 0, 0, 0));
            }
            break;
        case AST_VAR_LIST:
            result = tacJoin(tacJoin(code[0], code[1]),
            tacCreate(TAC_VAR_LIST, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_PARAM_LIST:
            result = tacJoin(tacJoin(code[0], code[1]),
            tacCreate(TAC_PARAM_LIST, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_PRINT_LIST:
            result = tacJoin(tacJoin(code[0], code[1]),
            tacCreate(TAC_PRINT_LIST, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_EXPR_LIST:
            result = tacJoin(tacJoin(code[0], code[1]),
            tacCreate(TAC_EXPR_LIST, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_CMD_PRINT:
            result = tacJoin(code[0], 
            tacCreate(TAC_CMD_PRINT, code[0] ? code[0]->resultado : 0, 0, 0));
            break;
        case AST_CMD_READ:
            result = tacJoin(code[0], 
            tacCreate(TAC_CMD_READ, code[0] ? code[0]->resultado : 0, 0, 0));
            break;
        case AST_FUNCALL:
            result = tacJoin(tacJoin(code[0], code[1]),
            tacCreate(TAC_FUNCALL, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_ADD:
            result = makeBinaryOp(TAC_ADD, code);
            break;
        case AST_SUB:
            result = makeBinaryOp(TAC_SUB, code);
            break;
        case AST_MUL:
            result = makeBinaryOp(TAC_MUL, code);
            break;
        case AST_DIV:
            result = makeBinaryOp(TAC_DIV, code);
            break;
        case AST_LESS:
            result = makeBinaryOp(TAC_LESS, code);
            break;
        case AST_LEQ:
            result = makeBinaryOp(TAC_LEQ, code);
            break;
        case AST_GREATER:
            result = makeBinaryOp(TAC_GREATER, code);
            break;
        case AST_GEQ:
            result = makeBinaryOp(TAC_GEQ, code);
            break;
        case AST_EQUAL:
            result = makeBinaryOp(TAC_EQUAL, code);
            break;
        case AST_NEQUAL:
            result = makeBinaryOp(TAC_NEQUAL, code);
            break;
        case AST_AND:
            result = makeBinaryOp(TAC_AND, code);
            break;
        case AST_OR:
            result = makeBinaryOp(TAC_OR, code);
            break;
        case AST_NOT:
            result = tacJoin(code[0],
            tacCreate(TAC_NOT,symbolMakeTemp(),code[0] ? code[0]->resultado : 0 ,code[1] ? code[1]->resultado : 0));
            break;
        case AST_CMD_ASSIGN:
            result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]),
            tacCreate(TAC_CMD_ASSIGN, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
            break;
        case AST_CMD_VEC_ASSIGN:
            result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]),
            tacCreate(TAC_CMD_VEC_ASSIGN, node->filho[0]->filho[0]->simbolo, code[1] ? code[1]->resultado : 0, code[2] ? code[2]->resultado : 0));
            break;
        default:
            result = tacJoin(code[0],tacJoin(code[1],tacJoin(code[2],code[3])));
            break;
            //fprintf(stderr, "Passei pelo tipo %d de AST.\n", node->tipo);
    }
    return result;
}