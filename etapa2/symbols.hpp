// Tabela de Símbolos - headers
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <string>
using namespace std;

// enum para os tipos de símbolos, baseada na vista em aula
enum SymbolType {
    SYMBOL_INVALID,
    SYMBOL_INT_FUNC,
    SYMBOL_REAL_FUNC,
    SYMBOL_BYTE_FUNC,
    SYMBOL_INT_VAR,
    SYMBOL_REAL_VAR,
    SYMBOL_BYTE_VAR,
    SYMBOL_INT_ID,
    SYMBOL_REAL_ID,
    SYMBOL_BYTE_ID,
    SYMBOL_INT_ARRAY,
    SYMBOL_REAL_ARRAY,
    SYMBOL_BYTE_ARRAY
};

// estrutura para os tipos de símbolos, baseada na vista em aula
struct SYMBOL{
    int type;
    string text;
    public:
        SYMBOL(int t, string s): type(t), text(s) {};
};

// declaração de funções
SYMBOL *symbolInsert(int type, char* text);
void symbolPrintTable();