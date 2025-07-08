// Geração de código ASM - hpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#ifndef ASM_HPP
#define ASM_HPP

#include "tac.hpp"
#include <stdio.h>

void asmGenerate(TAC* tac, FILE* out);
void asmGenerateCode(TAC* first, FILE* out);
void asmGenerateDataSection(TAC* first, FILE* out);
void asmReadVector(TAC* tac, FILE* out);
string getSymbolAddress(SYMBOL* s);

#endif

