// Análise semântica - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "semantic.hpp"

int semanticErrors = 0;

/*
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
*/

int semanticCheck(AST* nodo){
    if (nodo==0)
        return 0;
    switch(nodo->tipo){
        case AST_SYMBOL:{
            // verifica se o símbolo existe na tabela
            SYMBOL* s = symbolLookup(const_cast<char*>(nodo->simbolo->text.c_str()));
            if (s->type == SYMBOL_INVALID){
                fprintf(stderr, "Erro semântico: variável %s não declarada\n", nodo->simbolo->text.c_str());
                semanticErrors++;
            }
            break;
        }
        case AST_VAR_ATTR:{
            if (nodo->filho[0]->simbolo->dataType != DATA_ID){
                    fprintf(stderr, "Erro semântico: variável %s já declarada\n", nodo->filho[0]->simbolo->text.c_str());
                    semanticErrors++;
                }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT){
                nodo->filho[0]->simbolo = symbolInsert(SYMBOL_ID_INT, DATA_INT, const_cast<char*>(nodo->filho[0]->simbolo->text.c_str()));
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL){
                nodo->filho[0]->simbolo = symbolInsert(SYMBOL_ID_REAL, DATA_REAL, const_cast<char*>(nodo->filho[0]->simbolo->text.c_str()));
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_BYTE){
                nodo->filho[0]->simbolo = symbolInsert(SYMBOL_ID_BYTE, DATA_INT, const_cast<char*>(nodo->filho[0]->simbolo->text.c_str()));
            }

            if (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT){
                if (nodo->filho[1]->simbolo->type != SYMBOL_INT){
                    fprintf(stderr, "Erro semântico: variável %s não é do tipo inteiro\n", nodo->filho[0]->simbolo->text.c_str());
                    semanticErrors++;
                }
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL){
                if (nodo->filho[1]->simbolo->type != SYMBOL_REAL){
                    fprintf(stderr, "Erro semântico: variável %s não é do tipo real\n", nodo->filho[0]->simbolo->text.c_str());
                    semanticErrors++;
                }
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_BYTE){
                if (nodo->filho[1]->simbolo->type != SYMBOL_INT){
                    fprintf(stderr, "Erro semântico: variável %s não é do tipo byte\n", nodo->filho[0]->simbolo->text.c_str());
                    semanticErrors++;
                }
            }
            break;
        }
        case AST_VEC_ATTR:{
            if (nodo->filho[0]->simbolo->dataType != DATA_ID){
                    fprintf(stderr, "Erro semântico: variável %s já declarada\n", nodo->filho[0]->simbolo->text.c_str());
                    semanticErrors++;
                }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT){
                nodo->filho[0]->simbolo = symbolInsert(SYMBOL_ID_INT, DATA_VECTOR, const_cast<char*>(nodo->filho[0]->simbolo->text.c_str()));
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL){
                nodo->filho[0]->simbolo = symbolInsert(SYMBOL_ID_REAL, DATA_VECTOR, const_cast<char*>(nodo->filho[0]->simbolo->text.c_str()));
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_BYTE){
                nodo->filho[0]->simbolo = symbolInsert(SYMBOL_ID_BYTE, DATA_VECTOR, const_cast<char*>(nodo->filho[0]->simbolo->text.c_str()));
            }
            if (nodo->filho[1]->simbolo->dataType != DATA_INT){
                fprintf(stderr, "Erro semântico: tamanho do vetor %s não reduz a um tipo compatível com inteiro\n", nodo->filho[0]->simbolo->text.c_str());
                semanticErrors++;
            }
            if (nodo->filho.size() == 3){
                // TODO
            }
            break;
        }
    }

    for (uint32_t i=0; i<nodo->filho.size(); i++){
        semanticCheck(nodo->filho[i]);
    }
    return semanticErrors;
}