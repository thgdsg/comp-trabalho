%{
    int line_count = 1;
    int running = 1;
    void initMe(){
        
    };
    int isRunning(){
        return running;
    }
    int getLineNumber(){
        return line_count;
    }
    #include "parser.tab.hpp"
    #include "symbols.hpp"
    #include <cstring>
    // pra números inteiros (feito com char* pra compatibilidade com yytext)
    char* reverseInt(const char* s) {
        size_t len = strlen(s);
        char* reversed = new char[len + 1];
        // cria uma string nova e copia os caracteres na ordem inversa
        for (size_t i = 0; i < len; ++i) {
            reversed[i] = s[len - 1 - i];
        }
        reversed[len] = '\0';
        return reversed;
    }
    // pra números reais
    char* reverseRealNumber(const char* s) {
        // cria uma cópia da string pra tokenização
        char* copy = new char[strlen(s) + 1];
        strcpy(copy, s);
        
        // usa '/' como delimitador pra separar as duas partes
        char* part1 = strtok(copy, "/");
        char* part2 = strtok(NULL, "/");
        
        // inverte cada parte utilizando reverseInt
        char* rev1 = reverseInt(part1);
        char* rev2 = reverseInt(part2);
        
        // calcula o tamanho necessário pra a string resultado
        size_t len1 = strlen(rev1);
        size_t len2 = strlen(rev2);
        char* result = new char[len1 + len2 + 2]; // +1 pra barra e +1 pra '\0'
        
        strcpy(result, rev1);
        strcat(result, "/");
        strcat(result, rev2);
        
        // libera a memória alocada
        delete[] rev1;
        delete[] rev2;
        delete[] copy;
        
        return result;
    }
    // pra strings com "\n" no meio
    void addNewLines(const char* s, int* line_count) {
        for (size_t i = 0; i < std::strlen(s); ++i) {
            if (s[i] == '\n') {
                (*line_count)++;
            }
        }
    }
%}
%option nounput
%x COMMENT

%%
[0-9]+[/][0-9]+ {symbolInsert(SYMBOL_REAL,reverseRealNumber(yytext));return LIT_REAL;}
[0-9]+ {symbolInsert(SYMBOL_INT,reverseInt(yytext)); return LIT_INT;}
"byte" {return KW_BYTE;}
"int" {return KW_INT;}
"real" {return KW_REAL;}
"if" {return KW_IF;}
"else" {return KW_ELSE;}
"do" {return KW_DO;}
"while" {return KW_WHILE;}
"read" {return KW_READ;}
"print" {return KW_PRINT;}
"return" {return KW_RETURN;}
"/--" {BEGIN(COMMENT);}
[']([^'\\]|\\.)['] {symbolInsert(SYMBOL_CHAR,yytext); return LIT_CHAR;}
\"(\\.|[^\\\"\n])*\" {symbolInsert(SYMBOL_STRING,yytext); addNewLines(yytext,&line_count); return LIT_STRING;}
"//"[^\n]* {}
[A-Za-z_]+[A-Za-z0-9_]* {symbolInsert(SYMBOL_ID,yytext); return TK_IDENTIFIER;}
"<=" {return OPERATOR_LE;}
">=" {return OPERATOR_GE;}
"==" {return OPERATOR_EQ;}
"!=" {return OPERATOR_DIF;}
[-,;:()\[\]{}=+*/%<>&|~] {return yytext[0];}
"\n" {++line_count;}
" " {}
"\t" {}
. {return TOKEN_ERROR;}

<COMMENT>"\n" {++line_count;}
<COMMENT>. {}
<COMMENT>"--/" {BEGIN(INITIAL);}
%%


int yywrap(){
    running = 0;
    return 1;
}
