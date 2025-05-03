// Árvore de Sintaxe abstrata - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "ast.hpp"
#include "symbols.hpp"
#include <stdio.h>
#include <cstring>

string ASTTypeNames[] = {
    "AST_UNKNOWN",
    "AST_SYMBOL",
    "AST_DEF",
    "AST_VAR_ATTR", "AST_VEC_ATTR", "AST_FUN_ATTR",
    "AST_VAR_LIST", "AST_PARAM_LIST", "AST_EXPR_LIST", "AST_PRINT_LIST", "AST_CMD_LIST",
    "AST_CMD_IF", "AST_CMD_IFELSE", "AST_CMD_WHILE", "AST_CMD_DOWHILE",
    "AST_CMD_ASSIGN", "AST_CMD_VEC_ASSIGN", "AST_CMD_READ", "AST_CMD_PRINT", "AST_CMD_RETURN", 
    "AST_BLOCKCMD",
    "AST_VEC", "AST_FUNCALL",
    "AST_ADD", "AST_SUB", "AST_MUL", "AST_DIV",
    "AST_LESS", "AST_LEQ", "AST_GREATER", "AST_GEQ",
    "AST_EQUAL", "AST_NEQUAL",
    "AST_AND", "AST_OR",
    "AST_NOT",
};

void astPrint(AST* nodo, int nivel){
    for(int i=0; i<nivel; i++){
        printf("  ");
    }
    if (nodo==0) return;
    printf("AST: %s\n", ASTTypeNames[nodo->tipo].c_str());
    if (nodo->simbolo!=0) {
        for(int i=0; i<nivel; i++){
            printf("  ");
        }
        printf("Simbolo: %s, Tipo: %d\n", nodo->simbolo->text.c_str(), nodo->simbolo->type);
    }
    for (uint32_t i=0; i<nodo->filho.size(); i++){
        astPrint(nodo->filho[i], nivel+1);
    }
}

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

static void printIndent(FILE* out, int indent){
    for(int i=0;i<indent;i++) fputs("\t", out);
}

void astGenerate(AST* nodo, FILE* out, int indent){
    if (!nodo) return;
    switch(nodo->tipo){
        case AST_SYMBOL:{
            // identificador ou literal
            // caso sejam INTs ou REALs, inverte a string
            if (nodo->simbolo->type == SYMBOL_INT){
                char* inteiro = reverseInt(nodo->simbolo->text.c_str());
                fputs(inteiro, out);
            }
            else if (nodo->simbolo->type == SYMBOL_REAL){
                char* real = reverseRealNumber(nodo->simbolo->text.c_str());
                fputs(real, out);
            }
            else{
                fputs(nodo->simbolo->text.c_str(), out);
            }
            break;
        }
        case AST_VAR_ATTR:{
            if (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT){
                fputs("int ", out);
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL){
                fputs("real ", out);
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_BYTE){
                fputs("byte ", out);
            }
            astGenerate(nodo->filho[0], out, 0);
            fputs(" = ", out);
            astGenerate(nodo->filho[1], out, 0);
            fputs(";\n", out);
            break;
        }
        case AST_VEC_ATTR:{
            if (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT){
                fputs("int ", out);
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL){
                fputs("real ", out);
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_BYTE){
                fputs("byte ", out);
            }
            astGenerate(nodo->filho[0], out, 0);
            fputs(" [", out);
            astGenerate(nodo->filho[1], out, 0);
            fputs("]", out);
            if(nodo->filho.size() == 2){
                fputs(";\n", out);
            }
            else{
                fputs(" = ", out);
                astGenerate(nodo->filho[2], out, 0);
                fputs(";\n", out);
            }
            break;
        }
        case AST_FUN_ATTR:{
            if (nodo->filho[0]->simbolo->type == SYMBOL_ID_INT){
                fputs("int ", out);
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_REAL){
                fputs("real ", out);
            }
            else if (nodo->filho[0]->simbolo->type == SYMBOL_ID_BYTE){
                fputs("byte ", out);
            }
            astGenerate(nodo->filho[0], out, 0);
            if (nodo->filho.size() == 2){
                fputs(" (", out);
                fputs(")", out);
                astGenerate(nodo->filho[1], out, 0);
            }
            else{
                fputs(" (", out);
                astGenerate(nodo->filho[1], out, 0);
                fputs(" )", out);
                astGenerate(nodo->filho[2], out, 0);
            }
            break;
        }
        case AST_VAR_LIST:{
            astGenerate(nodo->filho[0], out, indent);
            if (nodo->filho.size() == 2){
                fputs(",", out);
                astGenerate(nodo->filho[1], out, indent);
            }
            break;
        }
        case AST_PARAM_LIST:{
            // imprime o tipo
            switch (nodo->filho[0]->simbolo->type) {
                case SYMBOL_ID_INT:
                    fputs("int ", out);
                    break;
                case SYMBOL_ID_REAL:
                    fputs("real ", out);
                    break;
                case SYMBOL_ID_BYTE:
                    fputs("byte ", out);
                    break;
            }
        
            // imprime o identificador
            astGenerate(nodo->filho[0], out, 0);
        
            // se tiver mais parâmetros, imprime vírgula e processa recursivamente
            if (nodo->filho.size() == 2) {
                fputs(", ", out);
                astGenerate(nodo->filho[1], out, 0);
            }
            break;
        }
        case AST_EXPR_LIST:{
            // expr , exprlist
            astGenerate(nodo->filho[0], out, 0);
            if (nodo->filho.size()>1){
                fputs(", ", out);
                astGenerate(nodo->filho[1], out, 0);
            }
            break;
        }
        case AST_PRINT_LIST:{
            // verifica se é um string
            if (nodo->filho[0]->tipo == AST_SYMBOL && nodo->filho[0]->simbolo) {
                fprintf(out, "%s", nodo->filho[0]->simbolo->text.c_str());
            } else {
                astGenerate(nodo->filho[0], out, 0);
            }

            // se houver mais itens, imprime espaço e processa o resto
            if (nodo->filho.size() == 2) {
                fputs(" ", out);
                astGenerate(nodo->filho[1], out, 0);
            }
            break;
        }
        case AST_CMD_LIST:{
            // sequência de comandos
            astGenerate(nodo->filho[0], out, indent);
            astGenerate(nodo->filho[1], out, indent);
            break;
        }
        case AST_CMD_IF:{
            printIndent(out, indent);
            fputs("if (", out);
            astGenerate(nodo->filho[0], out, indent);
            fputs(") ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_CMD_IFELSE:{
            printIndent(out, indent);
            fputs("if (", out);
            astGenerate(nodo->filho[0], out, 0);
            fputs(") ", out);
            astGenerate(nodo->filho[1], out, 0);
            printIndent(out, indent);
            fputs("else ", out);
            astGenerate(nodo->filho[2], out, 0);
            break;
        }
        case AST_CMD_WHILE:{
            printIndent(out, indent);
            fputs("while ", out);
            astGenerate(nodo->filho[0], out, indent);
            fputs(" do\n", out);
            astGenerate(nodo->filho[1], out, indent+1);
            break;
        }
        case AST_CMD_DOWHILE:{
            printIndent(out, indent);
            fputs("do\n", out);
            astGenerate(nodo->filho[0], out, indent+1);
            printIndent(out, indent);
            fputs("while ", out);
            astGenerate(nodo->filho[1], out, indent);
            fputs(";\n", out);
            break;
        }
        case AST_CMD_ASSIGN:{
            printIndent(out, indent);
            astGenerate(nodo->filho[0], out, 0);
            fputs(" = ", out);
            astGenerate(nodo->filho[1], out, 0);
            fputs(";\n", out);
            break;
        }
        case AST_CMD_VEC_ASSIGN:{
            printIndent(out, indent);
            astGenerate(nodo->filho[0], out, 0);
            fputs(" = ", out);
            astGenerate(nodo->filho[1], out, 0);
            fputs(";\n", out);
            break;
        }
        case AST_CMD_READ:{
            printIndent(out, indent);
            fputs("read ", out);
            astGenerate(nodo->filho[0], out, 0);
            fputs(";\n", out);
            break;
        }
        case AST_CMD_PRINT:{
            printIndent(out, indent);
            fputs("print ", out);
            astGenerate(nodo->filho[0], out, 0);
            fputs(";\n", out);
            break;
        }
        case AST_CMD_RETURN:{
            printIndent(out, indent);
            fputs("return ", out);
            astGenerate(nodo->filho[0], out, 0);
            fputs(";\n", out);
            break;
        }
        case AST_BLOCKCMD:{
            printIndent(out, indent);
            fputs("{\n", out);
            if (nodo->filho.size() > 0)
                astGenerate(nodo->filho[0], out, indent+1);
            printIndent(out, indent);
            fputs("}\n", out);
            break;
        }
        case AST_VEC:{
            astGenerate(nodo->filho[0], out, 0);
            fputc('[', out);
            astGenerate(nodo->filho[1], out, 0);
            fputc(']', out);
            break;
        }
        case AST_FUNCALL:{
            astGenerate(nodo->filho[0], out, 0);
            fputc('(', out);
            astGenerate(nodo->filho[1], out, 0);
            fputc(')', out);
            break;
        }
        case AST_ADD:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" + ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_SUB:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" - ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_MUL:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" * ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_DIV:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" / ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_LESS:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" < ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_LEQ:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" <= ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_GREATER:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" > ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_GEQ:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" >= ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_EQUAL:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" == ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_NEQUAL:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" != ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_AND:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" & ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_OR:{
            astGenerate(nodo->filho[0], out, 0);
            fputs(" | ", out);
            astGenerate(nodo->filho[1], out, 0);
            break;
        }
        case AST_NOT:{
            fputs("~", out);
            astGenerate(nodo->filho[0], out, 0);
            break;
        }
        default:
        // pros outros nós, apenas aplica recursão
        for(auto f : nodo->filho)
            astGenerate(f, out, indent);
        break;
    }
}

void astGenerateToFile(AST* raiz, const char* filename){
    FILE* out = fopen(filename, "w");
    if (!out) { perror("fopen"); return; }
    astGenerate(raiz, out, 0);
    fclose(out);
}