// Tabela de Símbolos - funções
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include "symbols.hpp"
#include <map>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

// tabela de Símbolos baseada na implementação feita em aula pelo professor
map<string,SYMBOL*> SymbolTable;
vector<string> symbolName = { "SYMBOL_INVALID", "SYMBOL_ID", "SYMBOL_INT", "SYMBOL_CHAR", "SYMBOL_REAL", "SYMBOL_STRING"};

// função para inserir símbolos na tabela, baseada na feita em aula
SYMBOL *symbolInsert(int type, char* text){
    if (SymbolTable.find(string(text)) != SymbolTable.end())
        return SymbolTable[string(text)];
    SYMBOL *newsymbol = new SYMBOL(type, string(text));
    SymbolTable[string(text)] = newsymbol;
    return newsymbol;
}

// função pra imprimir símbolos da tabela, baseada na feita em aula
void symbolPrintTable(){
    for (auto s : SymbolTable){
        int t = s.second->type;
        // dei cast em t de int pra size_t pra evitar warning de comparação entre signed e unsigned
        const auto name = (t >= 0 && static_cast<size_t>(t) < symbolName.size()) ? symbolName[t] : "ERRO/DEBUG";
        printf("Symbol[%s, %s]\n", name.c_str(), s.second->text.c_str());
    }
}