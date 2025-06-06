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
vector<string> symbolName = { "SYMBOL_INVALID", "SYMBOL_ID_INT", "SYMBOL_ID_BYTE", "SYMBOL_ID_REAL", "SYMBOL_INT", "SYMBOL_CHAR", "SYMBOL_REAL", "SYMBOL_STRING"};

// função para inserir símbolos na tabela, baseada na feita em aula
SYMBOL *symbolInsert(int type, char* text){
    // converte texto para chave
    std::string key(text);
    // busca na tabela
    auto it = SymbolTable.find(key);
    if (it != SymbolTable.end()) {
        // se achar mas tipo for inválido, remove e cria símbolo novo
        if (it->second->type == SYMBOL_INVALID) {
            delete it->second;
            SymbolTable.erase(it);
        }
        // se achar e tipo não for inválido, retorna o símbolo existente
        else {
            return it->second;
        }
    }
    // insere novo símbolo
    SYMBOL *newsymbol = new SYMBOL(type, key);
    SymbolTable[key] = newsymbol;
    return newsymbol;
}

// função para buscar símbolos na tabela (se não achar, cria um novo símbolo com tipo inválido)
SYMBOL* symbolLookup(char* text) {
    // utiliza o find() do map pra buscar o símbolo
    auto it = SymbolTable.find(string(text));
    if (it == SymbolTable.end()) {
        SYMBOL* a = symbolInsert(SYMBOL_INVALID, text);
        return a;
    }
    return it->second;
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