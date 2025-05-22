// Análise semântica - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "semantic.hpp"
#include <string>
#include <cstdlib>
#include <functional>

int semanticErrors = 0;

using namespace std;

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
    if (!nodo) return semanticErrors;
    // debug: qual nó está sendo verificado
    fprintf(stderr,
        "[semanticCheck] tipo=%d filhos=%zu\n",
        nodo->tipo, nodo->filho.size());

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
            if (nodo->filho.size() == 3) {
                // 1) obtém tamanho declarado (int ou char ASCII)
                AST* NodoTamanhoVec = nodo->filho[1];
                int declaredSize = 0;
                if (NodoTamanhoVec->tipo == AST_SYMBOL) {
                    SYMBOL* tamanhoVec = NodoTamanhoVec->simbolo;
                    // se for int, mais símples
                    if (tamanhoVec->type == SYMBOL_INT) {
                        // converte string de dígitos para inteiro
                        try {
                            declaredSize = std::stoi(tamanhoVec->text);
                        } catch (...) {
                            declaredSize = 0;
                        }
                    }
                    // se for char, converte para inteiro ASCII
                    else if (tamanhoVec->type == SYMBOL_CHAR) {
                        // texto esperado "'x'" ou "x"
                        const std::string& txt = tamanhoVec->text;
                        char c = (txt.size() >= 2 && txt.front()=='\'' && txt.back()=='\'')
                                 ? txt[1]
                                 : txt[0];
                        declaredSize = static_cast<unsigned char>(c);
                    }
                    else {
                        fprintf(stderr,
                            "Erro semântico: tamanho de vetor '%s' não reduz pra inteiro\n",
                            nodo->filho[0]->simbolo->text.c_str());
                        semanticErrors++;
                    }
                }

                // 2) percorre a lista de inicializadores, conta e verifica dataType
                AST* initList = nodo->filho[2];
                int elementCount = 0;
                // dataType esperado para cada elemento:
                int expectDataType = (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT)
                                    ? DATA_INT
                                  : (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL)
                                    ? DATA_REAL : DATA_INT;
                // percorre os filhos da lista de args da inicialização do vetor, vendo se ele é do tipo esperado
                // e também, se a quantidade de elementos é igual ao tamanho declarado
                function<void(AST*)> walk = [&](AST* cur) {
                    // caso base: nodo nulo, retorna
                    if (!cur) return;
                    //se for do tipo VAR_LIST, verifica os filhos
                    if (cur->tipo == AST_VAR_LIST) {
                        walk(cur->filho[0]);
                        walk(cur->filho[1]);
                    }
                    // se for um símbolo, verifica se ele é do tipo esperado e conta quantos já viu
                    else {
                        ++elementCount;
                        int litDataType = cur->simbolo->dataType;
                        if (litDataType != expectDataType) {
                            fprintf(stderr,
                                "Erro semântico: elemento %d de vetor '%s' tipo incompatível\n",
                                elementCount,
                                nodo->filho[0]->simbolo->text.c_str());
                            semanticErrors++;
                        }
                    }
                };
                walk(initList);

                // 3) compara quantidade real com a declarada
                if (elementCount != declaredSize) {
                    fprintf(stderr,
                        "Erro semântico: vetor '%s' declarado com tamanho %d mas inicializado com %d elementos\n",
                        nodo->filho[0]->simbolo->text.c_str(),
                        declaredSize, elementCount);
                    semanticErrors++;
                }
            }
            break;
        }
        case AST_FUN_ATTR:{
            // filho[0] = AST_SYMBOL da função, pra deixar mais legível
            SYMBOL* f = nodo->filho[0]->simbolo;
            if (f->dataType != DATA_ID){
                    fprintf(stderr, "Erro semântico: função %s já declarada\n", f->text.c_str());
                    semanticErrors++;
                }
            else if (f->type == SYMBOL_ID_INT){
                f = symbolInsert(SYMBOL_ID_INT, DATA_FUNCTION, const_cast<char*>(f->text.c_str()));
            }
            else if (f->type == SYMBOL_ID_REAL){
                f = symbolInsert(SYMBOL_ID_REAL, DATA_FUNCTION, const_cast<char*>(f->text.c_str()));
            }
            else if (f->type == SYMBOL_ID_BYTE) {
                f = symbolInsert(SYMBOL_ID_BYTE, DATA_FUNCTION, const_cast<char*>(f->text.c_str()));
            }
            // agora coleto parâmetros, mapeando o type → dataType real
            if (nodo->filho.size() >= 2) {
                f->paramTypes.clear();
                function<void(AST*)> collectParam = [&](AST* p){
                    if (!p) return;
                    auto& C = p->filho;
                    if (p->tipo==AST_PARAM_LIST){
                      if (C.size()>0) collectParam(C[0]);
                      if (C.size()>1) collectParam(C[1]);
                    }
                    else if (p->tipo==AST_SYMBOL){
                      int dt;
                      switch(p->simbolo->type){
                        case SYMBOL_ID_INT:  dt = DATA_INT;  break;
                        case SYMBOL_ID_REAL: dt = DATA_REAL; break;
                        case SYMBOL_ID_BYTE: dt = DATA_BOOL; break;  // ou DATA_STRING, conforme sua semântica
                        default:             dt = DATA_ID;   break;
                      }
                      f->paramTypes.push_back(dt);
                    }
                };
                collectParam(nodo->filho[1]);
            }
            break;
        }
        case AST_FUNCALL: {
            // filho[0] = AST_SYMBOL da função chamada
            SYMBOL* f = nodo->filho[0]->simbolo;
            if (f->dataType != DATA_FUNCTION) {
                fprintf(stderr,
                    "Erro semântico: '%s' não é função\n",
                    f->text.c_str());
                semanticErrors++;
                break;
            }
            // varre a lista de argumentos (filho[1]) e coleta tipos
            vector<int> args;
            if (nodo->filho.size()>1 && nodo->filho[1]){
              function<void(AST*)> collectArg = [&](AST* p){
                if (!p) return;
                auto& C = p->filho;
                if (p->tipo==AST_EXPR_LIST){
                  if (C.size()>0) collectArg(C[0]);
                  if (C.size()>1) collectArg(C[1]);
                }
                else {
                  // nunca acessar p->simbolo sem garantir que ele existe:
                  args.push_back(getDataType(p));
                }
              };
              collectArg(nodo->filho[1]);
            }

            // checa número de args
            if (args.size() != f->paramTypes.size()) {
                fprintf(stderr,
                "Erro semântico: função '%s' espera %zu args, recebeu %zu\n",
                f->text.c_str(),
                f->paramTypes.size(), args.size());
                semanticErrors++;
            }
            else {
                // checa tipo a tipo
                for (size_t i = 0; i < args.size(); ++i){
                    if (args[i] != f->paramTypes[i]) {
                        fprintf(stderr,
                        "Erro semântico: função '%s' arg #%zu tipo esperado %d, recebido %d\n",
                        f->text.c_str(), i+1,
                        f->paramTypes[i], args[i]);
                        semanticErrors++;
                    }
                }
            }
            break;
        }
        case AST_CMD_ASSIGN: {
            if (nodo->filho.size() != 2) {
              fprintf(stderr,
                "Internal error: AST_CMD_ASSIGN com %zu filhos\n",
                 nodo->filho.size());
              break;
            }
            AST* lhs = nodo->filho[0];
            AST* rhs = nodo->filho[1];
            if (lhs->tipo != AST_SYMBOL) {
              fprintf(stderr, "Internal error: comando de atribuição sem símbolo\n");
              break;
            }
            int L = getDataType(lhs);
            int R = getDataType(rhs);
            if (L == DATA_ID) {
              fprintf(stderr,
                "Erro semântico: variável %s não declarada na atribuição\n",
                lhs->simbolo->text.c_str());
              semanticErrors++;
            }
            else if (L != R) {
              fprintf(stderr,
                "Erro semântico: variável %s não é do tipo esperado\n",
                lhs->simbolo->text.c_str());
              semanticErrors++;
            }
            break;
        }
        case AST_CMD_VEC_ASSIGN: {
            if (nodo->filho.size() != 2) {
              fprintf(stderr,
                "Internal error: AST_CMD_VEC_ASSIGN com %zu filhos\n",
                 nodo->filho.size());
              break;
            }
            AST* vecNode = nodo->filho[0];
            AST* rhs     = nodo->filho[1];
            // AST_VEC tem dois filhos: [0]=símbolo do vetor, [1]=índice
            if (vecNode->tipo != AST_VEC || vecNode->filho.size() < 2) {
              fprintf(stderr, "Internal error: CMD_VEC_ASSIGN sem AST_VEC correto\n");
              break;
            }
            SYMBOL* s = vecNode->filho[0]->simbolo;
            int R = getDataType(rhs);
            if (s->dataType == DATA_ID) {
              fprintf(stderr,
                "Erro semântico: vetor %s não declarado na atribuição\n",
                s->text.c_str());
              semanticErrors++;
            }
            else if (s->dataType != R) {
              fprintf(stderr,
                "Erro semântico: vetor %s não é do tipo esperado\n",
                s->text.c_str());
              semanticErrors++;
            }
            break;
        }
        default:
            break;
    }

    // recursão segura: só itera até filho.size()
    for (size_t i = 0; i < nodo->filho.size(); i++){
      if (nodo->filho[i]) semanticCheck(nodo->filho[i]);
    }
    return semanticErrors;
}

// pega o dataType de qualquer sub-árvore de expressão
int getDataType(AST* expr){
    if (!expr) return DATA_ID;
    // pra debugging
    fprintf(stderr,
        "[getDataType] tipo=%d filhos=%zu\n",
        expr->tipo,
        expr->filho.size());

    switch(expr->tipo){
      case AST_SYMBOL:
        return expr->simbolo ? expr->simbolo->dataType : DATA_ID;
      case AST_VEC:
        if (expr->filho.size()>0 && expr->filho[0]->simbolo)
          return expr->filho[0]->simbolo->dataType;
        return DATA_ID;
      case AST_ADD: case AST_SUB:
      case AST_MUL: case AST_DIV:
      case AST_LESS: case AST_LEQ:
      case AST_GREATER: case AST_GEQ:
      case AST_EQUAL: case AST_NEQUAL:
      case AST_AND: case AST_OR: {
        int L = (expr->filho.size()>0) ? getDataType(expr->filho[0]) : DATA_ID;
        int R = (expr->filho.size()>1) ? getDataType(expr->filho[1]) : DATA_ID;
        return (L==R ? L : DATA_ID);
      }
      case AST_NOT:
        return (expr->filho.size()>0)
             ? getDataType(expr->filho[0])
             : DATA_ID;
      case AST_FUNCALL:
        if (expr->filho.size()>0 && expr->filho[0]->simbolo){
          switch(expr->filho[0]->simbolo->type){
            case SYMBOL_ID_INT:  return DATA_INT;
            case SYMBOL_ID_REAL: return DATA_REAL;
            default:             return DATA_ID;
          }
        }
        return DATA_ID;
      default:
        return DATA_ID;
    }
}