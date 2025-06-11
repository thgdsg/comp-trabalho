// Tabela de Símbolos - headers
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <string>
#include <vector>
#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP
using namespace std;

// enum para os tipos de símbolos, baseada na vista em aula
enum SymbolType {
    SYMBOL_INVALID, SYMBOL_ID_INT, SYMBOL_ID_BYTE, SYMBOL_ID_REAL, SYMBOL_INT, SYMBOL_CHAR, SYMBOL_REAL, SYMBOL_STRING
};

// enum para os tipos de dados, baseada na vista em aula
enum DataType {
    DATA_ID, DATA_INT, DATA_REAL, DATA_STRING, DATA_BOOL, DATA_FUNCTION, DATA_VECTOR
};

// estrutura para os tipos de símbolos, baseada na vista em aula
struct SYMBOL{
    int type;
    int dataType;
    string text;
    // paramTypes SÓ PRA FUNÇÕES
    vector<int> paramTypes;
    public:
        SYMBOL(int t, int d, string s): type(t), dataType(d), text(s) {};
};

// declaração de funções
SYMBOL *symbolInsert(int type, int dataType, char* text);
SYMBOL* symbolLookup(char* text);
void symbolPrintTable();
SYMBOL* symbolMakeTemp(void);
#endif