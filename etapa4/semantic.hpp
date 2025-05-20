// Análise semântica - hpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP
#include "ast.hpp"
#include "symbols.hpp"
#include <stdio.h>
#include <cstring>
#include <stdlib.h>

extern int semanticErrors;

void semanticCheck(AST* nodo);

#endif