// Análise semântica - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "semantic.hpp"

int semanticErrors = 0;

void semanticCheck(AST* nodo){
    if (nodo==0) return;
    switch(nodo->tipo){
        case AST_SYMBOL:{
            // verifica se o simbolo existe na tabela de simbolos
            SYMBOL* s = symbolLookup((char*)nodo->simbolo->text.c_str());
            if (s == 0){
                printf("Erro semantico: simbolo %s nao declarado\n", nodo->simbolo->text.c_str());
                semanticErrors++;
            }
            break;
        }
        case AST_DEF:{
            // verifica se o simbolo ja existe na tabela de simbolos
            SYMBOL* s = symbolLookup((char*)nodo->filho[0]->simbolo->text.c_str());
            if (s != 0){
                printf("Erro semantico: simbolo %s ja declarado\n", nodo->filho[0]->simbolo->text.c_str());
                semanticErrors++;
            }
            break;
        }
    }
    for (uint32_t i=0; i<nodo->filho.size(); i++){
        semanticCheck(nodo->filho[i]);
    }
}