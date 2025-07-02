// Geração de código ASM - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "asm.hpp"
#include "tac.hpp"
#include "symbols.hpp" // Incluir para ter acesso à SymbolTable
#include <iostream>
#include <string>
#include <map>

using namespace std;

// Mapa para associar literais de string a labels
static map<string, string> literalLabels; // Um único mapa para todos os literais
static int nextLiteralLabel = 0;

// Declara que a tabela de símbolos é externa
extern map<string, SYMBOL*> SymbolTable;

// Função auxiliar para obter o endereço de um símbolo
string getSymbolAddress(SYMBOL* s) {
    if (!s) return "0";

    // 1. Verifica se é um literal que foi armazenado na memória
    if (literalLabels.count(s->text)) {
        return literalLabels[s->text] + "(%rip)";
    }

    // 2. Se não foi armazenado, mas é um literal, trata como valor imediato
    //    (Isso pode não ser usado se o passo de pré-processamento pegar todos os literais)
    if (s->type == SYMBOL_INT || s->type == SYMBOL_REAL || s->type == SYMBOL_CHAR) {
        return "$" + s->text;
    }

    // 3. Assume que é uma variável global
    return s->text + "(%rip)";
}

void asmGenerateDataSection(TAC* first, FILE* out) {
    fprintf(out, "\t.section .data\n");

    for (auto it = SymbolTable.begin(); it != SymbolTable.end(); ++it) {
        const std::string& name = it->first;
        SYMBOL* symbol = it->second;

        if (symbol->dataType == DATA_FUNCTION) {
            continue;
        }

        // Trata temporários e variáveis não inicializadas com .comm (coloca na seção .bss)
        if (name.rfind("_temp", 0) == 0) {
            fprintf(out, "\t.comm %s,4,4\n", name.c_str());
        }

        // Trata variáveis globais (exceto vetores)
        else if (symbol->dataType != DATA_VECTOR && (symbol->type == SYMBOL_ID_INT || symbol->type == SYMBOL_ID_REAL || symbol->type == SYMBOL_ID_BYTE)) {
            fprintf(out, "\t.globl %s\n", name.c_str());
            fprintf(out, "%s:\n", name.c_str());

            switch (symbol->dataType) {
                case DATA_INT:
                    if (symbol->type == SYMBOL_ID_INT){
                        fprintf(out, "\t.align 4\n");
                        fprintf(out, "\t.long 0\n");
                    }
                    else if (symbol->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\t.align 1\n");
                        fprintf(out, "\t.byte 0\n");
                        break;
                    }
                case DATA_REAL:
                    fprintf(out, "\t.align 4\n");
                    fprintf(out, "\t.long 0\n");
                    fprintf(out, "\t.long 0\n");
                    break;
                default:
                    fprintf(stderr, "Warning: Unsupported data type for symbol '%s'.\n", name.c_str());
                    break;
            }
        }
    }

    // 2. Percorre as TACs para declarar vetores (com ou sem inicialização)
    for (TAC* tac = first; tac; tac = tac->next) {
        if (tac->tipo == TAC_VEC_ATTR) {
            if (!tac->resultado || !tac->op1) {
                fprintf(stderr, "Warning: Erro no TAC_VEC_ATTR!\n");
                continue;
            }
            SYMBOL* vecSymbol = tac->resultado;
            SYMBOL* sizeSymbol = tac->op1;
            int declaredSize = stoi(sizeSymbol->text);

            // Verifica se o símbolo já foi declarado para evitar duplicatas
            // (O loop acima pode ter pego variáveis com nomes parecidos)
            // Esta é uma simplificação. O ideal é ter um controle mais rígido.
            
            fprintf(out, "\t.globl %s\n", vecSymbol->text.c_str());
            fprintf(out, "\t.align 4\n");
            fprintf(out, "%s:\n", vecSymbol->text.c_str());

            if (tac->op2) { // Vetor inicializado
                vector<string> initValues;
                TAC* current = tac;
                for (int i = 0; i < declaredSize && current; ++i) {
                    SYMBOL* valSymbol = nullptr;
                    if (i == 0) { valSymbol = current->op2; } 
                    else {
                        current = current->prev;
                        if (current && current->tipo == TAC_VAR_LIST) {
                            valSymbol = current->resultado;
                        }
                    }
                    if (valSymbol) {
                        if (valSymbol->type == SYMBOL_CHAR) {
                            initValues.push_back(to_string((int)valSymbol->text[1]));
                        } else {
                            initValues.push_back(valSymbol->text);
                        }
                    } else { initValues.push_back("0"); }
                }
                for (auto it = initValues.rbegin(); it != initValues.rend(); ++it) {
                    fprintf(out, "\t.long %s\n", it->c_str());
                }
            } else { // Vetor não inicializado
                int spaceInBytes = declaredSize * 4;
                fprintf(out, "\t.space %d\n", spaceInBytes);
            }
        }
    }

    // 3. Declara os literais
    fprintf(out, "\n\t# Literais\n");
    for (auto const& pair : literalLabels) {
        const std::string& literalText = pair.first;
        const std::string& label = pair.second;

        // Obtém o ponteiro do símbolo para verificar seu tipo
        SYMBOL* s = symbolLookup(const_cast<char*>(literalText.c_str()));

        // Adiciona uma verificação de segurança
        if (!s) {
            fprintf(stderr, "Warning: Could not find symbol for literal '%s'. Skipping.\n", literalText.c_str());
            continue;
        }

        fprintf(out, "%s:\n", label.c_str());

        // Usa o dataType para escolher a diretiva assembly correta
        switch (s->dataType) {
            case DATA_INT:
                // Verifica se o literal é do tipo char para converter para ASCII
                if (s->type == SYMBOL_CHAR) {
                    // Extrai o caractere (ex: de "'a'" para 'a') e obtém seu valor ASCII
                    fprintf(out, "\t.align 4\n");
                    fprintf(out, "\t.long %d\n", (int)literalText[1]);
                } else {
                    // Se for um inteiro normal, apenas imprime o valor
                    fprintf(out, "\t.align 4\n");
                    fprintf(out, "\t.long %s\n", literalText.c_str());
                }
                break;
            case DATA_REAL:
            {
                // Divide o literal real "numerador/denominador" em duas partes
                size_t slash_pos = literalText.find('/');
                string numerator = "0";
                string denominator = "1"; // Evita divisão por zero se o formato for inesperado

                if (slash_pos != string::npos) {
                    numerator = literalText.substr(0, slash_pos);
                    denominator = literalText.substr(slash_pos + 1);
                } else {
                    // Adiciona um aviso se o formato não for o esperado
                    fprintf(stderr, "Warning: literal REAL '%s' possui um formato inesperado. Considerando como 0/1.\n", literalText.c_str());
                }

                fprintf(out, "\t.align 4\n");
                fprintf(out, "\t.long %s\n", numerator.c_str());
                fprintf(out, "\t.long %s\n", denominator.c_str());
                break;
            }
            case DATA_STRING:
                // A diretiva .string é para literais de texto
                fprintf(out, "\t.string %s\n", literalText.c_str());
                break;
            default:
                fprintf(stderr, "Warning: Literal '%s' with unknown dataType %d. Treating as string.\n", literalText.c_str(), s->dataType);
                fprintf(out, "\t.string %s\n", literalText.c_str());
                break;
        }
    }
}

void asmGenerateCode(TAC* first, FILE* out) {
    fprintf(out, "\t.section .text\n");
    fprintf(out, "\t.globl main\n"); // Assumindo que sempre há uma 'main'

    for (TAC* tac = first; tac; tac = tac->next) {
        switch (tac->tipo) {
            case TAC_FUNC_START:
                fprintf(out, "%s:\n", tac->resultado->text.c_str());
                fprintf(out, "\tpushq %%rbp\n");
                fprintf(out, "\tmovq %%rsp, %%rbp\n");
                break;

            case TAC_FUNC_END:
                fprintf(out, "\tleave\n");
                fprintf(out, "\tret\n");
                break;

            case TAC_CMD_ASSIGN:
                // Ex: movl op1, %eax; movl %eax, res
                fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_ADD:
                fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str());
                fprintf(out, "\taddl\t%%edx, %%eax\n");
                fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;
            
            case TAC_LABEL:
                fprintf(out, "%s:\n", tac->resultado->text.c_str());
                break;

            case TAC_JUMP_FALSE:
                // Compara o resultado da condição (op1) com 0
                fprintf(out, "\tcmpl\t$0, %s\n", getSymbolAddress(tac->op1).c_str());
                // Salta para o LABEL
                fprintf(out, "\tje\t%s\n", tac->resultado->text.c_str());
                break;

            case TAC_JUMP_TRUE:
                // Compara o resultado da condição (op1) com 0
                fprintf(out, "\tcmpl\t$0, %s\n", getSymbolAddress(tac->op1).c_str());
                // Salta para o LABEL
                fprintf(out, "\tjne\t%s\n", tac->resultado->text.c_str());
                break;

            default:
                //fprintf(stderr, "ASM generation not implemented for TAC type %d\n", tac->tipo);
                break;
        }
    }
}


void asmGenerate(TAC* tac, FILE* out) {
    if (!tac) return;

    
    // 1. Encontrar o início da lista de TACs
    TAC* first = tac;
    while (first && first->prev) {
        first = first->prev;
    }

    // 2. Pré-processar para encontrar TODOS os literais
    for (TAC* t = first; t; t = t->next) {
        auto processSymbol = [&](SYMBOL* s) {
            if (s && (s->type == SYMBOL_INT || s->type == SYMBOL_REAL || s->type == SYMBOL_CHAR || s->type == SYMBOL_STRING)) {
                if (literalLabels.find(s->text) == literalLabels.end()) {
                    literalLabels[s->text] = ".LIT" + to_string(nextLiteralLabel++);
                }
            }
        };
        
        processSymbol(t->resultado);
        processSymbol(t->op1);
        processSymbol(t->op2);
    }

    // 3. Gerar a seção de dados, passando a lista de TACs
    asmGenerateDataSection(first, out);

    // 4. Gerar a seção de código
    asmGenerateCode(first, out);
}

