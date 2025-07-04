// TAC - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "tac.hpp"

using namespace std;

string TACTypes[] = {
    "TAC_SYMBOL",
    "TAC_VAR_ATTR", "TAC_VEC_ATTR",
    "TAC_VAR_LIST", "TAC_PARAM_LIST", "TAC_EXPR_LIST", "TAC_PRINT_LIST",
    "TAC_CMD_ASSIGN", "TAC_CMD_VEC_ASSIGN", "TAC_CMD_READ", "TAC_CMD_PRINT", "TAC_CMD_RETURN",
    "TAC_VEC_READ", "TAC_FUNCALL",
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
    //if (tac->tipo == TAC_SYMBOL) return;

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

void tacPrintForwards(TAC* tac) {
    if (!tac) return;
    // encontra o início da lista
    while (tac->prev) tac = tac->prev;

    for (; tac; tac = tac->next) {
        tacPrintSingle(tac);
    }
}

TAC* tacJoin(TAC* l1, TAC* l2){
    if (!l1) return l2;
    if (!l2) return l1;

    TAC* point;

    for (point = l2; point->prev !=0; point = point->prev)
    ;

    point->prev = l1; // o início de l2 (point) aponta para trás para l1 (final da primeira lista)

    if (l1) {
        l1->next = point; // l1 (final da primeira lista) aponta para frente para o início de l2 (point)
    }

    return l2;
}

TAC* makeBinaryOp(int type, AST* node, TAC* code[]){
    SYMBOL* tipo_op = symbolLookup(const_cast<char*>(code[0]->resultado->text.c_str()));

    if (type == TAC_ADD || type == TAC_SUB || type == TAC_MUL || type == TAC_DIV){
        if (tipo_op->dataType == DATA_INT)
            return tacJoin(tacJoin(code[0],code[1]), tacCreate(type,symbolMakeTemp(SYMBOL_ID_INT, DATA_INT),code[0] ? code[0]->resultado : 0 ,code[1] ? code[1]->resultado : 0));
        if (tipo_op->dataType == DATA_REAL)
            return tacJoin(tacJoin(code[0],code[1]), tacCreate(type,symbolMakeTemp(SYMBOL_ID_REAL, DATA_REAL),code[0] ? code[0]->resultado : 0 ,code[1] ? code[1]->resultado : 0));
    }
    else if (type == TAC_LESS || type == TAC_LEQ || type == TAC_GREATER || type == TAC_GEQ ||
             type == TAC_EQUAL || type == TAC_NEQUAL || type == TAC_AND || type == TAC_OR) {
        return tacJoin(tacJoin(code[0], code[1]), tacCreate(type, symbolMakeTemp(SYMBOL_INVALID, DATA_BOOL), code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0));
    }
    else{
        fprintf(stderr, "Bug na operação binária!\n");
        return nullptr;
    }
    fprintf(stderr, "Bug catastrófico!\n");
    return nullptr;
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
    TAC* jumpToStart = tacCreate(TAC_JUMP_TRUE, startLabel, conditionCode ? conditionCode->resultado : nullptr, nullptr);
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
           // tacJoin(jumpTrueToStart, endLabelTac))));
}

TAC* makeFunction(TAC* code[]){
    SYMBOL* func_symbol = nullptr;
    TAC* params_code = nullptr;
    TAC* body_code = nullptr;

    func_symbol = code[0]->resultado;

    if (code[2] == nullptr){
        body_code = code[1];

        TAC* start_tac = tacCreate(TAC_FUNC_START, func_symbol, 0, nullptr);
        TAC* end_tac = tacCreate(TAC_FUNC_END, func_symbol, nullptr, nullptr);

        // encadeia: start_tac -> params_code (se existir) -> body_code -> end_tac
        TAC* result_code = start_tac;
        if (params_code) {
            result_code = tacJoin(result_code, params_code);
        }
        result_code = tacJoin(result_code, body_code);
        result_code = tacJoin(result_code, end_tac);

        return result_code;
    }
    else{
        params_code = code[1];
        body_code = code[2];

        TAC* start_tac = tacCreate(TAC_FUNC_START, func_symbol, code[1] ? code[1]->resultado : 0, nullptr);
        TAC* end_tac = tacCreate(TAC_FUNC_END, func_symbol, nullptr, nullptr);

        // encadeia: start_tac -> params_code (se existir) -> body_code -> end_tac
        TAC* result_code = start_tac;
        if (params_code) {
            result_code = tacJoin(result_code, params_code);
        }
        result_code = tacJoin(result_code, body_code);
        result_code = tacJoin(result_code, end_tac);

        return result_code;
    }
}

TAC* makeList(TAC* code[], int type){
    TAC* result = nullptr;
    if (type == TAC_VAR_LIST) {
        result = tacJoin(tacJoin(code[0], code[1]),
        tacCreate(type, code[0] ? code[0]->resultado : 0, 0, 0));
    } else if (type == TAC_PARAM_LIST) {
        result = tacJoin(tacJoin(code[0], code[1]),
        tacCreate(type, code[0] ? code[0]->resultado : 0, 0, 0));
    } else if (type == TAC_PRINT_LIST) {
        result = tacJoin(tacJoin(code[0], code[1]),
        tacCreate(type, code[0] ? code[0]->resultado : 0, 0, 0));
    } else if (type == TAC_EXPR_LIST) {
        result = tacJoin(tacJoin(code[0], code[1]),
        tacCreate(type, code[0] ? code[0]->resultado : 0, 0, 0));
    }
    
    return result;
}

TAC* makeAssign(TAC* code[], int type, AST* node){
    TAC* result = nullptr;

    if (type == TAC_CMD_ASSIGN) {
        result = tacJoin(tacJoin(code[0], code[1]),
    tacCreate(type, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0));
    } else if (type == TAC_CMD_VEC_ASSIGN) {
        result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]),
    tacCreate(type, node->filho[0]->filho[0]->simbolo, code[0] ? code[0]->op2 : 0, code[1] ? code[1]->resultado : 0));
    }

    return result;
}

TAC* makeAttr(TAC* code[], int type){
    TAC* result = nullptr;

    if (type == TAC_VAR_ATTR) {
        result = tacJoin(tacCreate(type, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, 0),
        code[0]);
    } else if (type == TAC_VEC_ATTR) {
        result = tacJoin(tacCreate(type, code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0, code[2] ? code[2]->resultado : 0),
        tacJoin(tacJoin(code[0], code[1]), code[2]));
    }

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
        {
            // Determina o tipo do elemento do vetor para criar um temporário correto.
            int elementType = DATA_ID;
            int elementSymbolType = SYMBOL_INVALID;
            if (node->filho[0] && node->filho[0]->simbolo) {
                switch (node->filho[0]->simbolo->type) {
                    case SYMBOL_ID_INT:  elementType = DATA_INT;  elementSymbolType = SYMBOL_ID_INT;  break;
                    case SYMBOL_ID_BYTE: elementType = DATA_INT;  elementSymbolType = SYMBOL_ID_BYTE; break; // Trata byte como int
                    case SYMBOL_ID_REAL: elementType = DATA_REAL; elementSymbolType = SYMBOL_ID_REAL; break;
                }
            }
            
            SYMBOL* temp = symbolMakeTemp(elementSymbolType, elementType);
            // Gera uma TAC para ler o valor do vetor[índice] para o temporário.
            result = tacJoin(
                tacJoin(code[0], code[1]),
                tacCreate(TAC_VEC_READ, temp, code[0]->resultado, code[1]->resultado)
            );
            break;
        }
        case AST_VAR_ATTR:
            result = makeAttr(code, TAC_VAR_ATTR);
            break;
        case AST_VEC_ATTR:
            result = makeAttr(code, TAC_VEC_ATTR);
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
            result = tacJoin(tacCreate(TAC_CMD_PRINT, code[0] ? code[0]->resultado : 0, 0, 0),
            code[0]);
            break;
        case AST_CMD_READ:
            result = tacJoin(code[0], 
            tacCreate(TAC_CMD_READ, code[0] ? code[0]->resultado : 0, 0, 0));
            break;
        case AST_FUNCALL:{
            SYMBOL* funcSymbol = code[0] ? code[0]->resultado : nullptr;
            if (funcSymbol) {
                int returnDataType = DATA_ID;
                int returnType = SYMBOL_INVALID;
                switch (funcSymbol->type) {
                    case SYMBOL_ID_INT:
                        returnDataType = DATA_INT;
                        returnType = SYMBOL_ID_INT;
                        break;
                    case SYMBOL_ID_BYTE:
                        returnDataType = DATA_INT;
                        returnType = SYMBOL_ID_BYTE;
                        break;
                    case SYMBOL_ID_REAL:
                        returnDataType = DATA_REAL;
                        returnType = SYMBOL_ID_REAL;
                        break;
                }
            SYMBOL* tempSymbol = symbolMakeTemp(returnType, returnDataType);
            result = tacJoin(tacJoin(code[0], code[1]),
            tacCreate(TAC_FUNCALL, tempSymbol, funcSymbol, 0));
            }
            else {
                fprintf(stderr, "Warning: Erro ao criar TAC da função! Criando função como INT.\n");
                result = tacJoin(tacJoin(code[0], code[1]),
                    tacCreate(TAC_FUNCALL, symbolMakeTemp(SYMBOL_ID_INT, DATA_INT), code[0] ? code[0]->resultado : 0, code[1] ? code[1]->resultado : 0));
            }
            break;
            }
        case AST_ADD:
            result = makeBinaryOp(TAC_ADD, node, code);
            break;
        case AST_SUB:
            result = makeBinaryOp(TAC_SUB, node, code);
            break;
        case AST_MUL:
            result = makeBinaryOp(TAC_MUL, node, code);
            break;
        case AST_DIV:
            result = makeBinaryOp(TAC_DIV, node, code);
            break;
        case AST_LESS:
            result = makeBinaryOp(TAC_LESS, node, code);
            break;
        case AST_LEQ:
            result = makeBinaryOp(TAC_LEQ, node, code);
            break;
        case AST_GREATER:
            result = makeBinaryOp(TAC_GREATER, node, code);
            break;
        case AST_GEQ:
            result = makeBinaryOp(TAC_GEQ, node, code);
            break;
        case AST_EQUAL:
            result = makeBinaryOp(TAC_EQUAL, node, code);
            break;
        case AST_NEQUAL:
            result = makeBinaryOp(TAC_NEQUAL, node, code);
            break;
        case AST_AND:
            result = makeBinaryOp(TAC_AND, node, code);
            break;
        case AST_OR:
            result = makeBinaryOp(TAC_OR, node, code);
            break;
        case AST_NOT:
            result = tacJoin(code[0],
            tacCreate(TAC_NOT,symbolMakeTemp(SYMBOL_INVALID, DATA_BOOL),code[0] ? code[0]->resultado : 0 ,code[1] ? code[1]->resultado : 0));
            break;
        case AST_CMD_ASSIGN:
            result = makeAssign(code, TAC_CMD_ASSIGN, nullptr);
            break;
        case AST_CMD_VEC_ASSIGN:
            result = makeAssign(code, TAC_CMD_VEC_ASSIGN, node);
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