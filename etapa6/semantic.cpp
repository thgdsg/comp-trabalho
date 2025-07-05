// Análise semântica - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "semantic.hpp"
#include <string>
#include <cstdlib>
#include <functional>

int semanticErrors = 0;
extern string ASTTypeNames[];
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

int semanticCheck(AST* nodo) {
    if (!nodo) return 0;

    semanticErrors += semanticCheck2Passadas(nodo, 1); // primeira passada
    semanticErrors += semanticCheck2Passadas(nodo, 2); // segunda passada

    return semanticErrors;
}

int semanticCheck2Passadas(AST* nodo, int pass){
  if (!nodo) return semanticErrors;
  // debug: qual nó está sendo verificado
  //fprintf(stderr,"[semanticCheck] tipo=%d filhos=%zu\n",nodo->tipo, nodo->filho.size());

  switch(nodo->tipo){
    case AST_SYMBOL:{
        if (pass == 1) {
          // verifica se o símbolo existe na tabela de símbolos
          SYMBOL* s = symbolLookup(const_cast<char*>(nodo->simbolo->text.c_str()));
            if (s->type == SYMBOL_INVALID){
                fprintf(stderr, "Erro semântico: variável %s não declarada\n", nodo->simbolo->text.c_str());
                semanticErrors++;
            }
          }
        break;
      }
      case AST_VAR_ATTR:{
        if (pass == 1) {
          // verifica se já passou por essa variável
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

          if (nodo->filho[0]->simbolo->dataType == DATA_INT){
              if (nodo->filho[1]->simbolo->dataType != DATA_INT){
                  fprintf(stderr, "Erro semântico: variável %s não é do tipo inteiro\n", nodo->filho[0]->simbolo->text.c_str());
                  semanticErrors++;
              }
          }
          else if (nodo->filho[0]->simbolo->dataType == DATA_REAL){
              if (nodo->filho[1]->simbolo->dataType != DATA_REAL){
                  fprintf(stderr, "Erro semântico: variável %s não é do tipo real\n", nodo->filho[0]->simbolo->text.c_str());
                  semanticErrors++;
              }
          }
        }
        break;
      }
      case AST_VEC_ATTR:{
        if (pass == 1) {
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
        }
        break;
      }
      case AST_FUN_ATTR:{
        if (pass == 1) {
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
          nodo->filho[0]->simbolo = f;
          //fprintf(stderr,"[semanticCheck] declarando função %s\n",f->text.c_str());
          // agora coleta parâmetros, mapeando o type -> dataType
          if (nodo->filho.size() > 2) {
            //fprintf(stderr,"[semanticCheck] coletando parâmetros da função %s\n",f->text.c_str());
              f->paramTypes.clear();
              // percorre a lista de parâmetros e rein­sere cada símbolo
              function<void(AST*)> collectParam = [&](AST* p){
                  if (!p) return;
                  auto& C = p->filho;
                  if (p->tipo == AST_PARAM_LIST) {
                      if (C.size()>0) collectParam(C[0]);
                      if (C.size()>1) collectParam(C[1]);
                  }
                  else if (p->tipo == AST_SYMBOL) {
                      // pega o tipo ID (SYMBOL_ID_*) e o texto
                      int  symType = p->simbolo->type;
                      auto name   = p->simbolo->text.c_str();
                      // determina o dataType correspondente
                      int dt;
                      switch(symType) {
                        case SYMBOL_ID_INT:  dt = DATA_INT;  break;
                        case SYMBOL_ID_REAL: dt = DATA_REAL; break;
                        case SYMBOL_ID_BYTE: dt = DATA_INT; break;
                        default:             dt = DATA_ID;   break;
                      }
                      // reinsere para atualizar dataType
                      SYMBOL* news = symbolInsert(symType, dt, const_cast<char*>(name));
                      p->simbolo = news;
                      f->paramTypes.push_back(dt);
                  }
              };
              collectParam(nodo->filho[1]);
          }
        }

        if (pass == 2) {
          SYMBOL* f = nodo->filho[0]->simbolo;
          //fprintf(stderr,"[semanticCheck] vendo return da função %s\n",f->text.c_str());
          // verifica pelo comando return dentro do corpo
          AST* body = (nodo->filho.size() >= 3 ? nodo->filho[2] : nullptr);
          bool foundReturn = false;
          function<void(AST*)> checkRet = [&](AST* n){
              if (!n) return;
              if (n->tipo == AST_CMD_RETURN) {
                  foundReturn = true;
                  AST* retExpr = (n->filho.size()>0 ? n->filho[0] : nullptr);
                  if (!retExpr) {
                      fprintf(stderr,
                          "Erro semântico: função %s não retorna expressão\n",
                          f->text.c_str());
                      semanticErrors++;
                  } else {
                      int actual   = getDataType(retExpr);
                      int expected = (f->type==SYMBOL_ID_INT  ? DATA_INT  :
                                      f->type==SYMBOL_ID_REAL ? DATA_REAL :
                                      /*byte/else*/           DATA_INT);
                      if (actual != expected) {
                          fprintf(stderr,
                              "Erro semântico: função %s retorna tipo %d, esperado %d\n",
                              f->text.c_str(), actual, expected);
                          semanticErrors++;
                      }
                  }
              }
              for (AST* c : n->filho) checkRet(c);
          };
          checkRet(body);
          if (!foundReturn) {
              fprintf(stderr,
                  "Warning: função %s não possui comando return\n",
                  f->text.c_str());
          }
        }
        break;
      }
      case AST_FUNCALL: {
        if (pass == 2) {
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
            "Erro semântico: função '%s' espera %zu args, recebeu %zu\n Declare a função antes de chamá-la no código.",
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
        }
        break;
      }
      case AST_CMD_ASSIGN: {
        if (pass == 2) {
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
        }
        break;
      }
      case AST_CMD_VEC_ASSIGN: {
        if (pass == 2) {
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
        else if (s->type == SYMBOL_ID_INT) {
          if (R != DATA_INT) {
            fprintf(stderr,
              "Erro semântico: vetor %s não é do tipo esperado\n",
              s->text.c_str());
            semanticErrors++;
          }
        }
        else if (s->type == SYMBOL_ID_REAL) {
          if (R != DATA_REAL) {
            fprintf(stderr,
              "Erro semântico: vetor %s não é do tipo esperado\n",
              s->text.c_str());
            semanticErrors++;
          }
        }
        else if (s->type == SYMBOL_ID_BYTE) {
          if (R != DATA_INT) {
            fprintf(stderr,
              "Erro semântico: vetor %s não é do tipo esperado\n",
              s->text.c_str());
            semanticErrors++;
          }
        }
        }
        break;
      }
      case AST_VEC:{
        if (pass == 1) {
        // filho[0] = AST_SYMBOL do vetor
        SYMBOL* s = nodo->filho[0]->simbolo;
        if (s->dataType != DATA_VECTOR) {
            fprintf(stderr,
                "Erro semântico: '%s' não é vetor\n",
                s->text.c_str());
            semanticErrors++;
        }
        // filho[1] = índice do vetor
        // se não for inteiro, erro
        if (nodo->filho.size() > 1) {
            int indexType = getDataType(nodo->filho[1]);
            if (indexType != DATA_INT) {
                fprintf(stderr, "Erro semântico: índice de vetor '%s' não é inteiro\n", s->text.c_str());
                semanticErrors++;
            }
        }
        }
          break;
      }
      case AST_CMD_IF:{
        if (pass == 1) {
        // filho[0] = expressão de teste
        // filho[1] = bloco de comandos
        if (nodo->filho.size() != 2) {
            fprintf(stderr, "Internal error: AST_CMD_IF com %zu filhos\n", nodo->filho.size());
            break;
        }
        int testType = getDataType(nodo->filho[0]);
        if (testType != DATA_BOOL) {
            fprintf(stderr, "Erro semântico: teste de if não é booleano\n");
            semanticErrors++;
        }
        }
          break;
      }
      case AST_CMD_IFELSE:{
        if (pass == 1) {
        // filho[0] = expressão de teste
        // filho[1] = bloco de comandos
        // filho[2] = bloco de comandos else
        if (nodo->filho.size() != 3) {
            fprintf(stderr, "Internal error: AST_CMD_IF com %zu filhos\n", nodo->filho.size());
            break;
        }
        int testType = getDataType(nodo->filho[0]);
        if (testType != DATA_BOOL) {
            fprintf(stderr, "Erro semântico: teste de if não é booleano\n");
            semanticErrors++;
        }
        }
          break;
      }
      case AST_CMD_WHILE:{
        if (pass == 1) {
        // filho[0] = expressão de teste
        // filho[1] = bloco de comandos
        if (nodo->filho.size() != 2) {
            fprintf(stderr, "Internal error: AST_CMD_WHILE com %zu filhos\n", nodo->filho.size());
            break;
        }
        int testType = getDataType(nodo->filho[0]);
        if (testType != DATA_BOOL) {
            fprintf(stderr, "Erro semântico: teste de while não é booleano\n");
            semanticErrors++;
        }
        }
          break;
      }
      case AST_CMD_DOWHILE:{
        if (pass == 1) {
        // filho[0] = bloco de comandos
        // filho[1] = expressão de teste
        if (nodo->filho.size() != 2) {
            fprintf(stderr, "Internal error: AST_CMD_DOWHILE com %zu filhos\n", nodo->filho.size());
            break;
        }
        int testType = getDataType(nodo->filho[1]);
        if (testType != DATA_BOOL) {
            fprintf(stderr, "Erro semântico: teste de do-while não é booleano\n");
            semanticErrors++;
        }
        }
          break;
      }
      default:
          break;
  }
  
  for (size_t i = 0; i < nodo->filho.size(); i++){
    if (nodo->filho[i]) semanticCheck2Passadas(nodo->filho[i], pass);
  }
  return semanticErrors;
}

// pega o dataType de qualquer sub-árvore de expressão
int getDataType(AST* expr){
  if (!expr) return DATA_ID;
  // pra debugging
  //fprintf(stderr,"[getDataType] tipo=%s filhos=%zu\n",ASTTypeNames[expr->tipo].c_str(),expr->filho.size());

  switch(expr->tipo){
    case AST_SYMBOL:
      return expr->simbolo ? expr->simbolo->dataType : DATA_ID;
    case AST_VEC:
      if (expr->filho.size()>0 && expr->filho[0]->simbolo){
        if (expr->filho[0]->simbolo->type == SYMBOL_ID_INT)
          return DATA_INT;
        if (expr->filho[0]->simbolo->type == SYMBOL_ID_REAL)
          return DATA_REAL;
        if (expr->filho[0]->simbolo->type == SYMBOL_ID_BYTE)
          return DATA_INT;
      }
      return DATA_ID;

    case AST_ADD: case AST_SUB:
    case AST_MUL: case AST_DIV:{
    int L = getDataType(expr->filho[0]);
    int R = getDataType(expr->filho[1]);
    // só soma/sub/mul/div int e real
    if ( (L==DATA_INT||L==DATA_REAL)
      && (L==R) )
      return L;
    return DATA_ID;
    }

    case AST_LESS: case AST_LEQ:
    case AST_GREATER: case AST_GEQ: {
    int L = getDataType(expr->filho[0]);
    int R = getDataType(expr->filho[1]);
    // só compara int ou real
    if ((L==DATA_INT||L==DATA_REAL) && L==R)
      return DATA_BOOL;
    return DATA_ID;
    }

    case AST_EQUAL: case AST_NEQUAL: {
    int L = getDataType(expr->filho[0]);
    int R = getDataType(expr->filho[1]);
    // compara qualquer tipo exceto string
    if (L==R && (L==DATA_INT||L==DATA_REAL||L==DATA_BOOL))
      return DATA_BOOL;
    return DATA_ID;
    }

    case AST_AND: case AST_OR: {
      int L = getDataType(expr->filho[0]);
      int R = getDataType(expr->filho[1]);
      if (L==DATA_BOOL && R==DATA_BOOL)
        return DATA_BOOL;
      return DATA_ID;
    }

    case AST_NOT: {
      int T = getDataType(expr->filho[0]);
      return (T==DATA_BOOL) ? DATA_BOOL : DATA_ID;
    }

    case AST_FUNCALL:
      if (expr->filho.size()>0 && expr->filho[0]->simbolo){
        switch(expr->filho[0]->simbolo->type){
          case SYMBOL_ID_INT:  return DATA_INT;
          case SYMBOL_ID_REAL: return DATA_REAL;
          case SYMBOL_ID_BYTE: return DATA_INT;
        }
      }
      return DATA_ID;
    
    default:
      return DATA_ID;
  }
}