// Geração de código ASM - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "asm.hpp"
#include "tac.hpp"
#include "symbols.hpp" // Incluir para ter acesso à SymbolTable
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// fazendo pro montador assembly AT&T rodando em linux

using namespace std;

// Mapa para associar literais de string a labels
static map<string, string> literalLabels; // Um único mapa para todos os literais
static int nextLiteralLabel = 0;

// num cabeçalho global
static vector<string> literalOrder;

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

void asmReadVector(TAC* tac, FILE* out){
    SYMBOL* vecSymbol = tac->op1;
    SYMBOL* indexSymbol = tac->op2;
    SYMBOL* resultSymbol = tac->resultado;

    fprintf(out, "\t# Lendo elemento de vetor: %s[%s]\n", vecSymbol->text.c_str(), indexSymbol->text.c_str());
    // Carrega o índice para um registrador (ex: %rcx)
    fprintf(out, "\tmovl\t%s, %%ecx\n", getSymbolAddress(indexSymbol).c_str());
    // Calcula o endereço do elemento e o coloca em %rax
    fprintf(out, "\tleaq\t%s(%%rip), %%rax\n", vecSymbol->text.c_str());

    // CORREÇÃO: Usa a instrução de movimentação correta com base no tipo do vetor.
    if (vecSymbol->type == SYMBOL_ID_BYTE) {
        // Para bytes, move 1 byte e zera o resto do registrador de destino.
        fprintf(out, "\tmovzbl\t(%%rax, %%rcx, 1), %%edx\n");
    } else if (vecSymbol->type == SYMBOL_ID_REAL) {
        // Para reais, move 8 bytes (numerador e denominador).
        // Assumindo que o resultado é um temporário do tipo real.
        fprintf(out, "\tmovq\t(%%rax, %%rcx, 8), %%rbx\n"); // Move 8 bytes para um registrador de 64 bits
        fprintf(out, "\tmovq\t%%rbx, %s\n", getSymbolAddress(resultSymbol).c_str());
        // O código abaixo é para separar em dois registradores, se necessário.
        // fprintf(out, "\tmovl\t(%%rax, %%rcx, 8), %%edx\n"); // Numerador
        // fprintf(out, "\tmovl\t4(%%rax, %%rcx, 8), %%r8d\n"); // Denominador
    }
    else { // Padrão para INT
        // Para inteiros, move 4 bytes.
        fprintf(out, "\tmovl\t(%%rax, %%rcx, 4), %%edx\n");
    }

    // Armazena o valor lido no temporário de resultado (exceto para real que já foi feito).
    if (vecSymbol->type != SYMBOL_ID_REAL) {
        fprintf(out, "\tmovl\t%%edx, %s\n", getSymbolAddress(resultSymbol).c_str());
    }
}

void asmGenerateDataSection(TAC* first, FILE* out) {
    // 0. Pré-processa TACs para capturar inicializações globais
    std::map<std::string, SYMBOL*> initValues;
    for (TAC* tac = first; tac && tac->tipo != TAC_FUNC_START; tac = tac->next) {
        if (tac->tipo == TAC_VAR_ATTR && tac->resultado && tac->op1) {
            const std::string& n = tac->resultado->text;
            if (n.rfind("_temp", 0) != 0) // não é temporária
                initValues[n] = tac->op1;
        }
    }

    fprintf(out, "\t.section .data\n");

    // 1. Declara variáveis escalares
    for (auto const& pair : SymbolTable) {
        const std::string& name = pair.first;
        SYMBOL* sym = pair.second;
        bool is_temp = name.rfind("_temp", 0) == 0;
        if (is_temp);
        else if (sym->dataType==DATA_FUNCTION||sym->dataType==DATA_VECTOR||
            sym->type==SYMBOL_INT||sym->type==SYMBOL_REAL||
            sym->type==SYMBOL_CHAR||sym->type==SYMBOL_STRING)
            continue;

        int size_in_bytes = 0;
        switch (sym->dataType) {
            case DATA_FUNCTION:
                if (sym->type==SYMBOL_ID_BYTE) size_in_bytes=1;
                else if (sym->type==SYMBOL_ID_REAL) size_in_bytes=8;
                else size_in_bytes=4;
                break;
            case DATA_INT:
                size_in_bytes = (sym->type==SYMBOL_ID_BYTE ? 1 : 4);
                break;
            case DATA_REAL:
                size_in_bytes = 8;
                break;
            case DATA_BOOL:
                size_in_bytes = 1;
                break;
            default:
                continue;
        }

        fprintf(out, "\t.globl %s\n", name.c_str());
        fprintf(out, "\t.align 4\n");
        fprintf(out, "%s:\n", name.c_str());

        auto it = initValues.find(name);
        if (it == initValues.end()) {
            // sem inicialização explícita: reserva espaço
            fprintf(out, "\t.space %d\n", size_in_bytes);
        }
        else {
            // inicializa com o valor capturado
            SYMBOL* val = it->second;
            if (sym->dataType == DATA_REAL) {
                // real = numerador/denominador
                size_t slash = val->text.find('/');
                std::string num = val->text.substr(0, slash);
                std::string den = (slash==std::string::npos? "1" : val->text.substr(slash+1));
                fprintf(out, "\t.long %s\n", num.c_str());
                fprintf(out, "\t.long %s\n", den.c_str());
            }
            else if (sym->type == SYMBOL_ID_BYTE) {
                // byte
                int v = std::stoi(val->text);
                fprintf(out, "\t.byte %d\n", v);
            }
            else {
                // int or bool
                fprintf(out, "\t.long %s\n", val->text.c_str());
            }
        }
    }

    // 2. Declarar vetores (mantém sua lógica existente) …
    //    (não alterado)
    // Percorre as TACs para declarar vetores (com ou sem inicialização)
    for (TAC* tac = first; tac; tac = tac->next) {
        if (tac->tipo == TAC_VEC_ATTR) {
            if (!tac->resultado || !tac->op1) {
                fprintf(stderr, "Warning: Erro no TAC_VEC_ATTR!\n");
                continue;
            }
            SYMBOL* vecSymbol = tac->resultado;
            SYMBOL* sizeSymbol = tac->op1;
            int declaredSize = stoi(sizeSymbol->text);
            
            fprintf(out, "\t.globl %s\n", vecSymbol->text.c_str());
            fprintf(out, "\t.align 4\n");
            fprintf(out, "%s:\n", vecSymbol->text.c_str());

            if (tac->op2) { // Vetor inicializado
                std::vector<std::string> final_vals;
                std::vector<std::string> tail_vals;
                
                // 1. Pega o primeiro valor da própria TAC_VEC_ATTR
                final_vals.push_back(tac->op2->text);

                // 2. Itera para FRENTE, coletando os valores da cauda (que estão invertidos)
                TAC* list_node = tac->next;
                while (list_node && final_vals.size() + tail_vals.size() < (long unsigned int)declaredSize) {
                    if (list_node->tipo != TAC_VAR_LIST) {
                        list_node = list_node->next;
                        continue;
                    }
                    if (list_node->resultado) {
                        tail_vals.push_back(list_node->resultado->text);
                    }
                    list_node = list_node->next;
                }

                // 3. Reverte a cauda para a ordem correta
                std::reverse(tail_vals.begin(), tail_vals.end());

                // 4. Adiciona a cauda ordenada à lista final
                final_vals.insert(final_vals.end(), tail_vals.begin(), tail_vals.end());

                // 5. A lista agora está na ordem correta, então imprimimos com base no tipo do vetor.
                if (vecSymbol->type == SYMBOL_ID_BYTE) {
                    for (const auto& val : final_vals) {
                        fprintf(out, "\t.byte %s\n", val.c_str());
                    }
                } else if (vecSymbol->type == SYMBOL_ID_REAL) {
                    for (const auto& val : final_vals) {
                        // Para reais, divide a string "num/den" e emite dois .long
                        size_t slash_pos = val.find('/');
                        string numerator = "0";
                        string denominator = "1";
                        if (slash_pos != string::npos) {
                            numerator = val.substr(0, slash_pos);
                            denominator = val.substr(slash_pos + 1);
                        } else {
                            // Se não houver '/', assume que é um número inteiro (ex: "5" -> 5/1)
                            numerator = val;
                        }
                        fprintf(out, "\t.long %s\n", numerator.c_str());
                        fprintf(out, "\t.long %s\n", denominator.c_str());
                    }
                } else { // Padrão para SYMBOL_ID_INT
                    for (const auto& val : final_vals) {
                        fprintf(out, "\t.long %s\n", val.c_str());
                    }
                }

            } else { // Vetor não inicializado
                int mult = 4; // Padrão para int
                if (vecSymbol->type == SYMBOL_ID_BYTE) mult = 1;
                else if (vecSymbol->type == SYMBOL_ID_REAL) mult = 8;
                
                int spaceInBytes = declaredSize * mult;
                fprintf(out, "\t.space %d\n", spaceInBytes);
            }
        }
    }

    // 3. Declara os literais E formatos de impressão para printf
    fprintf(out, "\n\t# Literais e Formatos de Impressao/Leitura\n");
    fprintf(out, ".LC_INT:\n");
    fprintf(out, "\t.string \"%%d \"\n"); // Formato para imprimir inteiros
    fprintf(out, ".LC_REAL:\n");
    fprintf(out, "\t.string \"%%d/%%d \"\n"); // Formato para imprimir reais
    fprintf(out, ".LC_CHAR:\n");
    fprintf(out, "\t.string \"%%c \"\n"); // Formato para imprimir chars
    fprintf(out, ".LC_STRING:\n");
    fprintf(out, "\t.string \"%%s\"\n"); // Formato para imprimir strings
    
    // NOVOS FORMATOS PARA SCANF
    fprintf(out, ".LC_SCAN_INT:\n");
    fprintf(out, "\t.string \"%%d\"\n"); // Formato para ler um inteiro
    fprintf(out, ".LC_SCAN_REAL:\n");
    fprintf(out, "\t.string \"%%d/%%d\"\n"); // Formato para ler um real (num/den)

    for (auto const& lit : literalOrder) {
      const auto &label = literalLabels[lit];
      fprintf(out, "%s:\n", label.c_str());
      SYMBOL* s = symbolLookup(const_cast<char*>(lit.c_str()));

      // Usa o dataType para escolher a diretiva assembly correta
      switch (s->dataType) {
          case DATA_INT:
              // Verifica se o literal é do tipo char para converter para ASCII
              if (s->type == SYMBOL_CHAR) {
                  // Extrai o caractere (ex: de "'a'" para 'a') e obtém seu valor ASCII
                  fprintf(out, "\t.align 4\n");
                  fprintf(out, "\t.long %d\n", (int)lit[1]);
              } else {
                  // Se for um inteiro normal, apenas imprime o valor
                  fprintf(out, "\t.align 4\n");
                  fprintf(out, "\t.long %s\n", lit.c_str());
              }
              break;
          case DATA_REAL:
          {
              // Divide o literal real "numerador/denominador" em duas partes
              size_t slash_pos = lit.find('/');
              string numerator = "0";
              string denominator = "1";

              if (slash_pos != string::npos) {
                  numerator = lit.substr(0, slash_pos);
                  denominator = lit.substr(slash_pos + 1);
              } else {
                  // Adiciona um aviso se o formato não for o esperado
                  fprintf(stderr, "Warning: literal REAL '%s' possui um formato inesperado. Considerando como 0/1.\n", lit.c_str());
              }

              fprintf(out, "\t.align 4\n");
              fprintf(out, "\t.long %s\n", numerator.c_str());
              fprintf(out, "\t.long %s\n", denominator.c_str());
              break;
          }
          case DATA_STRING:
              // A diretiva .string é para literais de texto
              fprintf(out, "\t.string %s\n", lit.c_str());
              break;
          default:
              fprintf(stderr, "Warning: Literal '%s' with unknown dataType %d. Treating as string.\n", lit.c_str(), s->dataType);
              fprintf(out, "\t.string %s\n", lit.c_str());
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
            {
                fprintf(out, "%s:\n", tac->resultado->text.c_str());
                fprintf(out, "\tpushq %%rbp\n");
                fprintf(out, "\tmovq %%rsp, %%rbp\n");

                // 1. Coletar parâmetros da lista de TACs
                vector<SYMBOL*> params;
                TAC* p_node = tac->next;
                while (p_node && (p_node->tipo == TAC_PARAM_LIST || p_node->tipo == TAC_SYMBOL)) {
                    if (p_node->tipo == TAC_PARAM_LIST && p_node->resultado) {
                        params.push_back(p_node->resultado);
                    }
                    p_node = p_node->next;
                }
                // A lista de TACs está invertida, então revertemos para a ordem correta dos argumentos.
                std::reverse(params.begin(), params.end());

                // 2. Mover argumentos dos registradores para as variáveis globais correspondentes.
                // A convenção de chamada x86-64 passa os primeiros 6 argumentos int/ponteiro nos registradores.
                const char* arg_regs_32[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
                const char* arg_regs_8[] = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
                size_t arg_reg_idx = 0; // Índice para percorrer os registradores de argumento

                for (size_t i = 0; i < params.size(); ++i) {
                    SYMBOL* p = params[i];
                    if (arg_reg_idx >= 6) {
                        fprintf(stderr, "ERRO! Número de argumentos excede o limite de 6 para registradores! Use menos argumentos em sua função.\n");
                        break;
                    }

                    fprintf(out, "\t# Movendo argumento %zu (%s) do registrador para a variavel global\n", i + 1, p->text.c_str());
                    
                    if (p->dataType == DATA_REAL) {
                        if (arg_reg_idx + 1 >= 6) {
                            fprintf(stderr, "ERRO! Parâmetro REAL excede o limite de registradores!\n");
                            break;
                        }
                        // Um REAL consome dois registradores
                        string addr = getSymbolAddress(p);
                        fprintf(out, "\tmovl\t%s, %s\n", arg_regs_32[arg_reg_idx], addr.c_str());         // Numerador
                        fprintf(out, "\tmovl\t%s, 4+%s\n", arg_regs_32[arg_reg_idx + 1], addr.c_str()); // Denominador
                        arg_reg_idx += 2;
                    } else if (p->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\tmovb\t%s, %s\n", arg_regs_8[arg_reg_idx], getSymbolAddress(p).c_str());
                        arg_reg_idx++;
                    } else { // INT
                        fprintf(out, "\tmovl\t%s, %s\n", arg_regs_32[arg_reg_idx], getSymbolAddress(p).c_str());
                        arg_reg_idx++;
                    }
                }
                // Avança o ponteiro tac para depois da lista de parâmetros
                if (p_node) {
                    tac = p_node->prev;
                }
                break;
            }
            case TAC_FUNC_END:
                fprintf(out, "\tleave\n");
                fprintf(out, "\tret\n");
                break;

            case TAC_CMD_ASSIGN:
                // Verifica se a atribuição é de um tipo REAL
                if (tac->op1 && tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Atribuição REAL\n");
                    // Move o numerador e o denominador (8 bytes)
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t4+%s, %%edx\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1 && (tac->op1->type == SYMBOL_ID_BYTE || tac->op1->type == SYMBOL_CHAR)) {
                    // Atribuição de BYTE (1 byte)
                    fprintf(out, "\t# Atribuição BYTE\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else {
                    // Atribuição padrão para INT (4 bytes)
                    fprintf(out, "\t# Atribuição INT\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
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

            case TAC_ADD:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real ADD\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\taddq\t%%rbx, %%rax\n"); // rax = a*d + c*b (novo numerador)
                    fprintf(out, "\tmovslq\t%%r9d, %%rcx\n\timulq\t%%r11, %%rcx\n");  // rcx = b*d (novo denominador)
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\taddl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_SUB:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real SUB\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tsubq\t%%rbx, %%rax\n"); // rax = a*d - c*b (novo numerador)
                    fprintf(out, "\tmovslq\t%%r9d, %%rcx\n\timulq\t%%r11, %%rcx\n");  // rcx = b*d (novo denominador)
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tsubl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_MUL:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real MUL\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r10, %%rax\n"); // rax = a*c
                    fprintf(out, "\tmovslq\t%%r9d, %%rcx\n\timulq\t%%r11, %%rcx\n");  // rcx = b*d
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\timull\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_DIV:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real DIV\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r9d, %%rcx\n\timulq\t%%r10, %%rcx\n");  // rcx = b*c
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcltd\n");
                    fprintf(out, "\tidivl\t%s\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_LESS:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real LESS\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tcmpq\t%%rbx, %%rax\n");
                    fprintf(out, "\tsetl\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcmpl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsetl\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;
            
            case TAC_GREATER:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real GREATER\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tcmpq\t%%rbx, %%rax\n");
                    fprintf(out, "\tsetg\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcmpl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsetg\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;
            
            case TAC_EQUAL:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real EQUAL\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tcmpq\t%%rbx, %%rax\n");
                    fprintf(out, "\tsete\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcmpl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsete\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_NEQUAL:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real NEQUAL\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tcmpq\t%%rbx, %%rax\n");
                    fprintf(out, "\tsetne\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcmpl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsetne\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_GEQ:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real GEQ\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tcmpq\t%%rbx, %%rax\n");
                    fprintf(out, "\tsetge\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcmpl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsetge\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_LEQ:
                if (tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Real LEQ\n");
                    fprintf(out, "\tmovl\t%s, %%r8d\n", getSymbolAddress(tac->op1).c_str());      // a
                    fprintf(out, "\tmovl\t4+%s, %%r9d\n", getSymbolAddress(tac->op1).c_str());    // b
                    fprintf(out, "\tmovl\t%s, %%r10d\n", getSymbolAddress(tac->op2).c_str());     // c
                    fprintf(out, "\tmovl\t4+%s, %%r11d\n", getSymbolAddress(tac->op2).c_str());   // d
                    fprintf(out, "\tmovslq\t%%r8d, %%rax\n\timulq\t%%r11, %%rax\n"); // rax = a*d
                    fprintf(out, "\tmovslq\t%%r10d, %%rbx\n\timulq\t%%r9, %%rbx\n");  // rbx = c*b
                    fprintf(out, "\tcmpq\t%%rbx, %%rax\n");
                    fprintf(out, "\tsetle\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tcmpl\t%s, %%eax\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsetle\t%%al\n");
                    fprintf(out, "\tmovzbl\t%%al, %%eax\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_AND:
                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                fprintf(out, "\tandb\t%%bl, %%al\n");
                fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_OR:
                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                fprintf(out, "\torb\t%%bl, %%al\n");
                fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_NOT:
                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tnotb\t%%al\n");
                fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_VEC_READ:
                asmReadVector(tac, out);
                break;
            case TAC_PRINT_LIST:
            {
                // 1) Coleta todos os nós TAC_PRINT_LIST consecutivos
                vector<SYMBOL*> printed_values;
                TAC* node = tac;
                while (node && node->tipo == TAC_PRINT_LIST) {
                    if (node->resultado)
                        printed_values.push_back(node->resultado);
                    node = node->next;
                }
                // Guarda o último PRINT_LIST processado
                TAC* last = node ? node->prev : tac;

                // 2) Inversão manual do vetor
                for (size_t i = 0, j = printed_values.size() - 1; i < j; ++i, --j) {
                    SYMBOL* tmp = printed_values[i];
                    printed_values[i] = printed_values[j];
                    printed_values[j] = tmp;
                }

                // 3) Gera o código de print para cada item na ordem correta
                for (SYMBOL* item : printed_values) {
                    if (!item) continue;
                    fprintf(out, "\t# Imprimindo valor: %s\n", item->text.c_str());
                    if (item->type == SYMBOL_STRING) {
                        // Usa printf com formato "%s" para não adicionar newline automaticamente.
                        fprintf(out, "\tleaq\t.LC_STRING(%%rip), %%rdi\n"); // 1º arg: formato "%s"
                        fprintf(out, "\tleaq\t%s, %%rsi\n", getSymbolAddress(item).c_str()); // 2º arg: a string
                        fprintf(out, "\tmovl\t$0, %%eax\n"); 
                        fprintf(out, "\tcall\tprintf@PLT\n");
                    }
                    else if (item->dataType == DATA_REAL) {
                        string addr = getSymbolAddress(item);
                        fprintf(out, "\tmovl\t%s, %%esi\n", addr.c_str());
                        fprintf(out, "\tmovl\t4+%s, %%edx\n", addr.c_str());
                        fprintf(out, "\tleaq\t.LC_REAL(%%rip), %%rdi\n");
                        fprintf(out, "\tmovl\t$0, %%eax\n");
                        fprintf(out, "\tcall\tprintf@PLT\n");
                    }
                    else if (item->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\tmovzbl\t%s, %%esi\n", getSymbolAddress(item).c_str());
                        fprintf(out, "\tleaq\t.LC_INT(%%rip), %%rdi\n");
                        fprintf(out, "\tmovl\t$0, %%eax\n");
                        fprintf(out, "\tcall\tprintf@PLT\n");
                    }
                    else {
                        fprintf(out, "\tmovl\t%s, %%esi\n", getSymbolAddress(item).c_str());
                        fprintf(out, "\tleaq\t.LC_INT(%%rip), %%rdi\n");
                        fprintf(out, "\tmovl\t$0, %%eax\n");
                        fprintf(out, "\tcall\tprintf@PLT\n");
                    }
                }

                // 4) Ajusta 'tac' para pular todos os nós que acabamos de processar
                tac = last;
                break;
            }
            case TAC_CMD_PRINT:
                break;
            case TAC_FUNCALL:
            {
                // 1. Coletar argumentos das TACs TAC_EXPR_LIST anteriores.
                vector<SYMBOL*> args;
                TAC* current = tac->prev;
                while (current && current->tipo == TAC_EXPR_LIST) {
                    if (current->resultado) {
                        args.push_back(current->resultado);
                    }
                    current = current->prev;
                }
                // A lista de argumentos está agora na ordem correta para os registradores (arg1, arg2, ...)

                // 2. Carregar argumentos nos registradores de acordo com a convenção x86-64.
                const char* arg_regs_32[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
                size_t arg_reg_idx = 0;

                fprintf(out, "\t# Preparando argumentos para a chamada de %s\n", tac->op1->text.c_str());
                for (SYMBOL* arg : args) {
                    if (arg_reg_idx >= 6) {
                        fprintf(stderr, "ERRO: Mais de 6 argumentos não são suportados.\n");
                        break;
                    }

                    if (arg->dataType == DATA_REAL) {
                        if (arg_reg_idx + 1 >= 6) {
                            fprintf(stderr, "ERRO: Argumento REAL excede o limite de registradores.\n");
                            break;
                        }
                        // Passa o numerador no primeiro registrador e o denominador no segundo.
                        fprintf(out, "\tmovl\t%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx]);
                        fprintf(out, "\tmovl\t4+%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx + 1]);
                        arg_reg_idx += 2;
                    } else if (arg->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\tmovzbl\t%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx]);
                        arg_reg_idx++;
                    } else { // INT
                        fprintf(out, "\tmovl\t%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx]);
                        arg_reg_idx++;
                    }
                }

                // 3. Chamar a função. O nome da função está em op1.
                fprintf(out, "\tcall\t%s\n", tac->op1->text.c_str());

                // 4. Obter o valor de retorno e armazená-lo no resultado (temporário).
                SYMBOL* funcSymbol = tac->op1;
                SYMBOL* resultSymbol = tac->resultado;

                if (resultSymbol) {
                    fprintf(out, "\t# Armazenando valor de retorno de %s em %s\n", funcSymbol->text.c_str(), resultSymbol->text.c_str());
                    // Verifica o tipo de retorno da função para saber de onde copiar o resultado.
                    switch (funcSymbol->type) {
                        case SYMBOL_ID_REAL:
                            // Reais (8 bytes) são retornados em %eax (numerador) e %edx (denominador).
                            fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(resultSymbol).c_str());
                            fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(resultSymbol).c_str());
                            break;

                        case SYMBOL_ID_BYTE:
                            // Bytes (1 byte) são retornados na parte baixa de %rax.
                            fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(resultSymbol).c_str());
                            break;

                        case SYMBOL_ID_INT:
                        default:
                            // Inteiros (4 bytes) são retornados em %eax.
                            fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(resultSymbol).c_str());
                            break;
                    }
                }
                break;
            }
            case TAC_CMD_RETURN:
            {
                if (tac->resultado) {
                    // Verifica o tipo de dado do valor a ser retornado.
                    switch (tac->resultado->dataType) {
                        case DATA_REAL:
                            // Carrega o numerador em %eax e o denominador em %edx.
                            fprintf(out, "\t# Retornando REAL\n");
                            fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->resultado).c_str());
                            fprintf(out, "\tmovl\t4+%s, %%edx\n", getSymbolAddress(tac->resultado).c_str());
                            break;

                        case DATA_VECTOR:
                            // Vetores não são retornados diretamente, mas podemos retornar o endereço do vetor.
                            fprintf(out, "\t# Retornando endereço de vetor\n");
                            fprintf(out, "\tmovq\t%s, %%rax\n", getSymbolAddress(tac->resultado).c_str());
                            break;
                        case DATA_INT:
                        default:
                            // Verifica se o tipo específico é BYTE para usar movb.
                            if (tac->resultado->type == SYMBOL_ID_BYTE) {
                                fprintf(out, "\t# Retornando BYTE\n");
                                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->resultado).c_str());
                            } else{
                                // Caso contrário, trata como um INT padrão.
                                fprintf(out, "\t# Retornando INT\n");
                                fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->resultado).c_str());
                            }
                            break;
                    }
                }
                // Após carregar o valor de retorno, executa o epílogo da função.
                fprintf(out, "\tleave\n");
                fprintf(out, "\tret\n");
                break;
            }
            case TAC_CMD_VEC_ASSIGN:
            {
                SYMBOL* vecSymbol = tac->resultado;
                SYMBOL* indexSymbol = tac->op1;
                SYMBOL* valueSymbol = tac->op2;

                fprintf(out, "\t# Atribuindo a elemento de vetor: %s[%s] = %s\n", vecSymbol->text.c_str(), indexSymbol->text.c_str(), valueSymbol->text.c_str());

                // Carrega o índice para %rcx
                fprintf(out, "\tmovl\t%s, %%ecx\n", getSymbolAddress(indexSymbol).c_str());
                // Carrega o endereço base do vetor para %rax
                fprintf(out, "\tleaq\t%s(%%rip), %%rax\n", vecSymbol->text.c_str());


                // Trata a atribuição com base no tipo do vetor
                if (vecSymbol->type == SYMBOL_ID_REAL) {
                    // Carrega o numerador e denominador do valor a ser atribuído
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(valueSymbol).c_str());      // Numerador
                    fprintf(out, "\tmovl\t4+%s, %%r8d\n", getSymbolAddress(valueSymbol).c_str()); // Denominador
                    // Armazena o numerador na posição correta
                    fprintf(out, "\tmovl\t%%edx, (%%rax, %%rcx, 8)\n");
                    // Armazena o denominador 4 bytes depois
                    fprintf(out, "\tmovl\t%%r8d, 4(%%rax, %%rcx, 8)\n");
                } else if (vecSymbol->type == SYMBOL_ID_BYTE) {
                    // Carrega o valor (byte) para a parte baixa de %edx
                    fprintf(out, "\tmovb\t%s, %%dl\n", getSymbolAddress(valueSymbol).c_str());
                    // Armazena o byte na posição correta
                    fprintf(out, "\tmovb\t%%dl, (%%rax, %%rcx, 1)\n");
                } else { // Padrão para INT
                    // Carrega o valor (inteiro) para %edx
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(valueSymbol).c_str());
                    // Armazena o inteiro na posição correta
                    fprintf(out, "\tmovl\t%%edx, (%%rax, %%rcx, 4)\n");
                }
                break;
            }
            case TAC_VEC_ATTR:
            case TAC_VAR_ATTR:
                // lido com isso na hora de gerar a data section
                break;
            case TAC_CMD_READ:
            {
                SYMBOL* var = tac->resultado;
                if (!var) break;

                fprintf(out, "\t# Lendo valor para a variavel %s\n", var->text.c_str());

                // A convenção de chamada para scanf(formato, ...args) é:
                // 1. %rdi <- endereço da string de formato
                // 2. %rsi, %rdx, ... <- endereços das variáveis para armazenar os dados
                // 3. %eax <- 0 (para funções variádicas)

                if (var->dataType == DATA_REAL) {
                    // Caso REAL: formato "%d/%d", precisa de dois ponteiros de destino.
                    // %rsi -> endereço do numerador
                    // %rdx -> endereço do denominador
                    fprintf(out, "\tleaq\t.LC_SCAN_REAL(%%rip), %%rdi\n");
                    fprintf(out, "\tleaq\t%s, %%rsi\n", getSymbolAddress(var).c_str());
                    fprintf(out, "\tleaq\t4+%s, %%rdx\n", getSymbolAddress(var).c_str());
                } else {
                    // Caso INT ou BYTE: formato "%d", precisa de um ponteiro de destino.
                    // %rsi -> endereço da variável
                    fprintf(out, "\tleaq\t.LC_SCAN_INT(%%rip), %%rdi\n");
                    fprintf(out, "\tleaq\t%s, %%rsi\n", getSymbolAddress(var).c_str());
                }

                // Zera %eax antes de chamar uma função variádica como scanf
                fprintf(out, "\tmovl\t$0, %%eax\n");
                fprintf(out, "\tcall\tscanf@PLT\n");
                break;
            }
        default:
                //fprintf(stderr, "ASM generation not implemented for TAC type %d\n", tac->tipo);
                break;
        }
    }

    // Funções auxiliares em Assembly
    fprintf(out, "\n# --- Funcoes Auxiliares ---\n");

    // _gcd: Calcula o Máximo Divisor Comum usando o algoritmo de Euclides.
    // Entrada: %rdi, %rsi
    // Saída: %rax
    fprintf(out, "_gcd:\n");
    fprintf(out, "\tmovq\t%%rdi, %%rax\n");
    fprintf(out, "\tmovq\t%%rsi, %%rbx\n");
    fprintf(out, "_gcd_loop:\n");
    fprintf(out, "\tcmpq\t$0, %%rbx\n");
    fprintf(out, "\tje\t_gcd_end\n");
    fprintf(out, "\tmovq\t%%rbx, %%rcx\n");
    fprintf(out, "\tmovq\t%%rax, %%rdx\n");
    fprintf(out, "\tmovq\t$0, %%rax\n"); // Limpa rax para a divisão
    fprintf(out, "\tmovq\t%%rdx, %%rax\n");
    fprintf(out, "\tmovq\t$0, %%rdx\n"); // Limpa rdx para o resto
    fprintf(out, "\tidivq\t%%rcx\n");
    fprintf(out, "\tmovq\t%%rcx, %%rax\n");
    fprintf(out, "\tmovq\t%%rdx, %%rbx\n");
    fprintf(out, "\tjmp\t_gcd_loop\n");
    fprintf(out, "_gcd_end:\n");
    fprintf(out, "\tret\n");

    // _simplify_fraction: Simplifica uma fração.
    // Entrada: Numerador em %rdi, Denominador em %rsi
    // Saída: Numerador simplificado em %rax, Denominador simplificado em %rdx
    fprintf(out, "_simplify_fraction:\n");
    fprintf(out, "\tpushq\t%%rdi\n");
    fprintf(out, "\tpushq\t%%rsi\n");
    fprintf(out, "\tcall\t_gcd\n"); // gcd(%rdi, %rsi) -> %rax
    fprintf(out, "\tpopq\t%%rsi\n"); // rsi = denominador original
    fprintf(out, "\tpopq\t%%rdi\n"); // rdi = numerador original
    fprintf(out, "\tmovq\t%%rax, %%rbx\n"); // rbx = gcd
    fprintf(out, "\tmovq\t%%rdi, %%rax\n");
    fprintf(out, "\tmovq\t$0, %%rdx\n");
    fprintf(out, "\tidivq\t%%rbx\n"); // rax = numerador / gcd
    fprintf(out, "\tpushq\t%%rax\n"); // Salva numerador simplificado
    fprintf(out, "\tmovq\t%%rsi, %%rax\n");
    fprintf(out, "\tmovq\t$0, %%rdx\n");
    fprintf(out, "\tidivq\t%%rbx\n"); // rax = denominador / gcd
    fprintf(out, "\tmovq\t%%rax, %%rdx\n"); // rdx = denominador simplificado
    fprintf(out, "\tpopq\t%%rax\n"); // rax = numerador simplificado
    fprintf(out, "\tret\n");
}

void asmGenerate(TAC* tac, FILE* out) {
    if (!tac) return;

    // adiciona a seção .note.GNU-stack pra evitar o warning do compilador
    fprintf(out, "\t.section\t.note.GNU-stack,\"\",@progbits\n");
    
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
                    literalOrder.push_back(s->text);
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
    fprintf(stderr, "Arquivo ASM gerado!\n");
}

