// main
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "symbols.hpp"

extern FILE* yyin;
extern int isRunning();
extern int getLineNumber();
int yylex();
extern int running;
extern int line_count;

int main(int argc, char** argv){
    if (argc<2){
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    if((yyin=fopen(argv[1], "r"))==0){
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
    int token = 0;
    while(isRunning()){
        if (!isRunning()){
            break;
        }
        token = yylex();
        switch (token){
            case KW_BYTE:
                printf("KW_BYTE ");
                break;
            case KW_INT:
                printf("KW_INT ");
                break;
            case KW_REAL:
                printf("KW_REAL");
                break;
            case KW_IF:
                printf("KW_IF ");
                break;
            case KW_ELSE:
                printf("KW_ELSE ");
                break;
            case KW_DO:
                printf("KW_DO ");
                break;
            case KW_WHILE:
                printf("KW_WHILE ");
                break;
            case KW_READ:
                printf("KW_READ ");
                break;
            case KW_PRINT:
                printf("KW_PRINT ");
                break;
            case KW_RETURN:
                printf("KW_RETURN ");
                break;
            case OPERATOR_LE:
                printf("OPERATOR_LE ");
                break;
            case OPERATOR_GE:
                printf("OPERATOR_GE ");
                break;
            case OPERATOR_EQ:
                printf("OPERATOR_EQ ");
                break;
            case OPERATOR_DIF:
                printf("OPERATOR_DIF ");
                break;
            case TK_IDENTIFIER:
                printf("TK_IDENTIFIER ");
                break;
            case LIT_INT:
                printf("LIT_INT ");
                break;
            case LIT_CHAR:
                printf("LIT_CHAR ");
                break;
            case LIT_REAL:
                printf("LIT_REAL ");
                break;
            case LIT_STRING:
                printf("LIT_STRING ");
                break;
            case TOKEN_ERROR:
                printf("TOKEN_ERROR ");
                break;
            case 0:
                printf("\nFim do Arquivo\n");
                running = 0;
                break;
            default:
                printf("%c ", token);
                break;
        }
    }
    fprintf(stderr,"File has %d lines\n", getLineNumber());
    symbolPrintTable();
}