// TAC - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "tac.hpp"

using namespace std;

string TACTypes[] = {
    "TAC_SYMBOL",
    "TAC_VAR_ATTR", "TAC_VEC_ATTR", "TAC_FUN_ATTR",
    "TAC_VAR_LIST", "TAC_PARAM_LIST", "TAC_EXPR_LIST", "TAC_PRINT_LIST",
    "TAC_CMD_IF", "TAC_CMD_IFELSE", "TAC_CMD_WHILE", "TAC_CMD_DOWHILE",
    "TAC_CMD_ASSIGN", "TAC_CMD_VEC_ASSIGN", "TAC_CMD_READ", "TAC_CMD_PRINT", "TAC_CMD_RETURN",
    "TAC_VEC", "TAC_FUNCALL",
    "TAC_ADD", "TAC_SUB", "TAC_MUL", "TAC_DIV",
    "TAC_LESS", "TAC_LEQ", "TAC_GREATER", "TAC_GEQ",
    "TAC_EQUAL", "TAC_NEQUAL",
    "TAC_AND", "TAC_OR",
    "TAC_NOT",
    "TAC_LABEL",
    "TAC_JUMP_FALSE", "TAC_JUMP_TRUE",
    "TAC_FUNC_START", "TAC_FUNC_END"
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

TAC* makeIfThen(TAC* code[]){
    TAC* jumpTAC = nullptr;
    TAC* labelTAC = nullptr;
    SYMBOL* newlabel = nullptr;

    newlabel = symbolMakeLabel();

    jumpTAC = tacCreate(TAC_JUMP_FALSE,newlabel,code[0] ? code[0]->resultado : 0,0);
    labelTAC = tacCreate(TAC_LABEL,newlabel,0,0);

    // Encadeia as TACs nessa ordem:
    // 1. Código da condição
    // 2. Salto se falso pro próximo comando após o if
    // 3. Código do bloco then
    // 4. Label do próximo comando após o if
    return tacJoin(tacJoin(tacJoin(code[0], jumpTAC),code[1]),labelTAC);
}

TAC* makeIfThenElse(TAC* code[]){
    SYMBOL* elseLabel = symbolMakeLabel();
    SYMBOL* endIfLabel = symbolMakeLabel();

    TAC* jumpFalseToElse = tacCreate(TAC_JUMP_FALSE, elseLabel, code[0] ? code[0]->resultado : nullptr, nullptr);
    TAC* unconditionalJumpToEnd = tacCreate(TAC_JUMP_TRUE, endIfLabel, nullptr, nullptr); // O label de destino está em 'res'
    TAC* elseLabelTac = tacCreate(TAC_LABEL, elseLabel, nullptr, nullptr);
    TAC* endIfLabelTac = tacCreate(TAC_LABEL, endIfLabel, nullptr, nullptr);

    // Encadeia as TACs nessa ordem:
    // 1. Código da condição
    // 2. Salto se falso para o label do else
    // 3. Código do bloco then
    // 4. Salto incondicional para o label de fim do if
    // 5. Label do else
    // 6. Código do bloco else
    // 7. Label de fim do if
    return tacJoin(code[0],
           tacJoin(jumpFalseToElse,
           tacJoin(code[1],
           tacJoin(unconditionalJumpToEnd,
           tacJoin(elseLabelTac,
           tacJoin(code[2], endIfLabelTac))))));
}

TAC* makeWhile(TAC* code[]){
    SYMBOL* startLabel = symbolMakeLabel();
    SYMBOL* endLabel = symbolMakeLabel();

    TAC* conditionCode = code[0];
    TAC* bodyCode = code[1];

    TAC* startLabelTac = tacCreate(TAC_LABEL, startLabel, nullptr, nullptr);
    TAC* jumpFalseToEnd = tacCreate(TAC_JUMP_FALSE, endLabel, conditionCode ? conditionCode->resultado : nullptr, nullptr);
    TAC* jumpToStart = tacCreate(TAC_JUMP_TRUE, startLabel, nullptr, nullptr);
    TAC* endLabelTac = tacCreate(TAC_LABEL, endLabel, nullptr, nullptr);

    // Encadeia as TACs:
    // 1. Label de início do loop
    // 2. Código da condição
    // 3. Salto se falso para o label de fim do loop
    // 4. Código do corpo do loop
    // 5. Salto incondicional para o label de início do loop
    // 6. Label de fim do loop
    return tacJoin(startLabelTac,
           tacJoin(conditionCode,
           tacJoin(jumpFalseToEnd,
           tacJoin(bodyCode,
           tacJoin(jumpToStart, endLabelTac)))));
}

TAC* makeDoWhile(TAC* code[]){
    SYMBOL* startLabel = symbolMakeLabel();
    // SYMBOL* endLabel = symbolMakeLabel(); // Not strictly necessary for do-while's basic form

    TAC* bodyCode = code[0];
    TAC* conditionCode = code[1];

    TAC* startLabelTac = tacCreate(TAC_LABEL, startLabel, nullptr, nullptr);
    TAC* jumpTrueToStart = tacCreate(TAC_JUMP_TRUE, startLabel, conditionCode ? conditionCode->resultado : nullptr, nullptr);
    // TAC* endLabelTac = tacCreate(TAC_LABEL, endLabel, nullptr, nullptr); // If an end label is desired

    // Encadeia as TACs:
    // 1. Label de início do loop
    // 2. Código do corpo do loop
    // 3. Código da condição
    // 4. Salto se verdadeiro para o label de início do loop
    return tacJoin(startLabelTac,
           tacJoin(bodyCode,
           tacJoin(conditionCode, jumpTrueToStart)));
           // tacJoin(conditionCode, 
           // tacJoin(jumpTrueToStart, endLabelTac)))); // If using an end label
}

TAC* makeFunction(TAC* code[]){
    SYMBOL* func_symbol = nullptr;
    TAC* params_code = nullptr;
    TAC* body_code = nullptr;

    func_symbol = code[0]->resultado;

    if (code[2] == nullptr){
        body_code = code[1];
    }
    else{
        params_code = code[1];
        body_code = code[2];
    }

    TAC* start_tac = tacCreate(TAC_FUNC_START, func_symbol, nullptr, nullptr);
    TAC* end_tac = tacCreate(TAC_FUNC_END, func_symbol, nullptr, nullptr);

    // Encadeia: start_tac -> params_code (se existir) -> body_code -> end_tac
    TAC* result_code = start_tac;
    if (params_code) {
        result_code = tacJoin(result_code, params_code);
    }
    result_code = tacJoin(result_code, body_code);
    result_code = tacJoin(result_code, end_tac);

    return result_code;
}

TAC* makeList(TAC* code[], int type){
    TAC* result = nullptr;

    result = tacJoin(tacJoin(code[0], code[1]),
    tacCreate(type, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
    
    return result;
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
            result = makeFunction(code);
            break;
        case AST_VAR_LIST:
            result = makeList(code, TAC_VAR_LIST);
            break;
        case AST_PARAM_LIST:
            result = makeList(code, TAC_PARAM_LIST);
            break;
        case AST_PRINT_LIST:
            result = makeList(code, TAC_PRINT_LIST);
            break;
        case AST_EXPR_LIST:
            result = makeList(code, TAC_EXPR_LIST);
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
        case AST_CMD_RETURN:
            result = tacJoin(code[0],
            tacCreate(TAC_CMD_RETURN, code[0] ? code[0]->resultado : 0, 0, 0));
            break;
        case AST_CMD_IF:
            result = makeIfThen(code);
            break;
        case AST_CMD_IFELSE:
            result = makeIfThenElse(code);
            break;
        case AST_CMD_WHILE:
            result = makeWhile(code);
            break;
        case AST_CMD_DOWHILE:
            result = makeDoWhile(code);
            break;
        default:
            result = tacJoin(code[0],tacJoin(code[1],tacJoin(code[2],code[3])));
            break;
            //fprintf(stderr, "Passei pelo tipo %d de AST.\n", node->tipo);
    }
    return result;
}