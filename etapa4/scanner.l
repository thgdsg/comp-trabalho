%{
// scanner
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

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
    #include "ast.hpp"
    #include <cstring>
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
[0-9]+[/][0-9]+ {yylval.symbol = symbolInsert(SYMBOL_REAL,DATA_REAL,reverseRealNumber(yytext)); return LIT_REAL;}
[0-9]+ {yylval.symbol = symbolInsert(SYMBOL_INT,DATA_INT,reverseInt(yytext)); return LIT_INT;}
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
[']([^'\\]|\\.)['] {yylval.symbol = symbolInsert(SYMBOL_CHAR,DATA_CHAR,yytext); return LIT_CHAR;}
\"(\\.|[^\\\"\n])*\" {yylval.symbol = symbolInsert(SYMBOL_STRING,DATA_STRING,yytext); addNewLines(yytext,&line_count); return LIT_STRING;}
"//"[^\n]* {}
[A-Za-z_]+[A-Za-z0-9_]* { yylval.sval = strdup(yytext); return TK_IDENTIFIER; }
"<=" {return OPERATOR_LE;}
">=" {return OPERATOR_GE;}
"==" {return OPERATOR_EQ;}
"!=" {return OPERATOR_DIF;}
[-,;:()\[\]{}=+*/%<>&|~] {return yytext[0];}
"\n" {++line_count;}
" " {}
"\t" {}
. {}

<COMMENT>"\n" {++line_count;}
<COMMENT>. {}
<COMMENT>"--/" {BEGIN(INITIAL);}
%%


int yywrap(){
    running = 0;
    return 1;
}
