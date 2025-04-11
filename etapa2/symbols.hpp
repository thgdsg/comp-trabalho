// Tabela de Símbolos - headers
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <string>
using namespace std;

// enum para os tipos de símbolos, baseada na vista em aula
enum SymbolType {
    SYMBOL_INVALID, SYMBOL_ID_INT, SYMBOL_ID_BYTE, SYMBOL_ID_REAL, SYMBOL_INT, SYMBOL_CHAR, SYMBOL_REAL, SYMBOL_STRING
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