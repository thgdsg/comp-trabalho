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
vector<string> symbolName = { "SYMBOL_INVALID", "SYMBOL_ID_INT", "SYMBOL_ID_BYTE", "SYMBOL_ID_REAL", "SYMBOL_INT", "SYMBOL_CHAR", "SYMBOL_REAL", "SYMBOL_STRING", "SYMBOL_LABEL"};
vector<string> dataType = { "DATA_ID", "DATA_INT", "DATA_REAL", "DATA_STRING", "DATA_BOOL", "DATA_FUNCTION", "DATA_VECTOR"};

// função para inserir símbolos na tabela, baseada na feita em aula
SYMBOL *symbolInsert(int type, int dataType, char* text){
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
        else {
            // se já existe e datatype ainda não foi atribuído
            if (it->second->dataType == DATA_ID && dataType != 0) {
                it->second->dataType = dataType;
            }
            return it->second;
        }
    }
    // insere novo símbolo
    SYMBOL *newsymbol = new SYMBOL(type, dataType, key);
    SymbolTable[key] = newsymbol;
    return newsymbol;
}

// função para buscar símbolos na tabela (se não achar, cria um novo símbolo com tipo inválido)
SYMBOL* symbolLookup(char* text) {
    // utiliza o find() do map pra buscar o símbolo
    auto it = SymbolTable.find(string(text));
    if (it == SymbolTable.end()) {
        SYMBOL* a = symbolInsert(SYMBOL_INVALID, 0, text);
        return a;
    }
    return it->second;
}

// função pra imprimir símbolos da tabela, baseada na feita em aula
void symbolPrintTable(){
     for (auto s : SymbolTable){
        int t  = s.second->type;
        int dt = s.second->dataType;
        // nome do tipo léxico
        const auto tipo = (t >= 0 && static_cast<size_t>(t) < symbolName.size())
            ? symbolName[t] : "ERRO/DEBUG";
        // nome do dataType
        const auto tipoDado = (dt >= 0 && static_cast<size_t>(dt) < dataType.size())
            ? dataType[dt] : "ERRO/DEBUG";
 
        // começa a imprimir
        printf("Symbol[%s, %s, %s", tipo.c_str(), tipoDado.c_str(), s.second->text.c_str());
        // se tiver parâmetros, imprime eles
        if (!s.second->paramTypes.empty()) {
            printf(", params=");
            for (size_t i = 0; i < s.second->paramTypes.size(); ++i) {
                int p = s.second->paramTypes[i];
                // nome do tipo léxico do parâmetro
                const auto pName = (p >= 0 && static_cast<size_t>(p) < symbolName.size())
                    ? symbolName[p] : "ERRO/DEBUG";
                printf("%s%s",
                    pName.c_str(),
                    (i + 1 < s.second->paramTypes.size()) ? "," : "");
            }
        }
        // fecha colchete e pula linha
        printf("]\n");
    }
}

SYMBOL* symbolMakeTemp(void) {
    static int serialNumber = 0;
    static char buffer[20] = "";
    snprintf(buffer, sizeof(buffer), "temp%d", serialNumber++);
    SYMBOL* tempSymbol = symbolInsert(SYMBOL_INVALID, DATA_ID, buffer);
    return tempSymbol;
}

SYMBOL* symbolMakeLabel(void){
    static int serialNumber = 0;
    static char buffer[20] = "";
    snprintf(buffer, sizeof(buffer), "label%d", serialNumber++);
    SYMBOL* labelSymbol = symbolInsert(SYMBOL_LABEL, DATA_ID, buffer);
    return labelSymbol;
}