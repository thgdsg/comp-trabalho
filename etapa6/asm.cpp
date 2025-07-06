// Geração de código ASM - cpp
// Thiago dos Santos Gonçalves - 00344587
// Semestre 2025/1

#include "asm.hpp"
#include "tac.hpp"
#include "symbols.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// fazendo pro montador assembly AT&T rodando em linux

using namespace std;

// map global pra associar literais de string a labels no código
static map<string, string> literalLabels;
static int nextLiteralLabel = 0;

// com ordem global pra manter a ordem de inserção dos literais
static vector<string> literalOrder;

// pega a tabela de símbolos pra gerar declarações de variáveis
extern map<string, SYMBOL*> SymbolTable;


// função auxiliar pra obter o endereço de um símbolo
string getSymbolAddress(SYMBOL* s) {
    if (!s) return "0";

    // verifica se é um literal que foi armazenado na memória
    if (literalLabels.count(s->text)) {
        return literalLabels[s->text] + "(%rip)";
    }

    // se não, assume que é uma variável global ou temporária
    return s->text + "(%rip)";
}

void asmReadVector(TAC* tac, FILE* out){
    SYMBOL* vecSymbol = tac->op1;
    SYMBOL* indexSymbol = tac->op2;
    SYMBOL* resultSymbol = tac->resultado;

    fprintf(out, "\t# Lendo elemento de vetor: %s[%s]\n", vecSymbol->text.c_str(), indexSymbol->text.c_str());

    // carrega o índice pro registrador %ecx
    fprintf(out, "\tmovl\t%s, %%ecx\n", getSymbolAddress(indexSymbol).c_str());
    // calcula o endereço do elemento e o coloca em %rax
    fprintf(out, "\tleaq\t%s(%%rip), %%rax\n", vecSymbol->text.c_str());

    // usa a instrução de movimentação com base no tipo do vetor
    if (vecSymbol->type == SYMBOL_ID_BYTE) {
        // pra bytes, move 1 byte e zera o resto do registrador de destino
        fprintf(out, "\tmovzbl\t(%%rax, %%rcx, 1), %%edx\n");
    } else if (vecSymbol->type == SYMBOL_ID_REAL) {
        // pra reais, move 8 bytes (numerador e denominador)
        fprintf(out, "\tmovq\t(%%rax, %%rcx, 8), %%rbx\n");
        fprintf(out, "\tmovq\t%%rbx, %s\n", getSymbolAddress(resultSymbol).c_str());
    }
    else { // caso padrão: INT
        // move 4 bytes
        fprintf(out, "\tmovl\t(%%rax, %%rcx, 4), %%edx\n");
    }

    // armazena o valor lido no temporário de resultado (exceto pra real que já foi feito)
    if (vecSymbol->type != SYMBOL_ID_REAL) {
        fprintf(out, "\tmovl\t%%edx, %s\n", getSymbolAddress(resultSymbol).c_str());
    }
}

// gera toda a seção de dados do código assembly
// declara variáveis globais, vetores e literais
// também declara os formatos de impressão e leitura usados pelo printf/scanf
void asmGenerateDataSection(TAC* first, FILE* out) {
    // pré-processa TACs pra capturar inicializações globais
    std::map<std::string, SYMBOL*> initValues;
    for (TAC* tac = first; tac && tac->tipo != TAC_FUNC_START; tac = tac->next) {
        if (tac->tipo == TAC_VAR_ATTR && tac->resultado && tac->op1) {
            const std::string& n = tac->resultado->text;
            if (n.rfind("_temp", 0) != 0) // não é temporária
                initValues[n] = tac->op1;
        }
    }

    fprintf(out, "\t.section .data\n");

    // declara variáveis escalares
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

    // 2. declara vetores
    // percorre as TACs pra declarar vetores
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

            if (tac->op2) { // vetor inicializado
                std::vector<std::string> final_vals;
                std::vector<std::string> tail_vals;
                
                // pega o primeiro valor da própria TAC_VEC_ATTR
                final_vals.push_back(tac->op2->text);

                // itera pra frente, coletando os valores invertidos da cauda
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

                // reverte o vetor pra ordem correta
                std::reverse(tail_vals.begin(), tail_vals.end());

                // adiciona a cauda ordenada a lista final
                final_vals.insert(final_vals.end(), tail_vals.begin(), tail_vals.end());

                // com a lista na ordem correta, imprime no ASM com base no tipo do vetor
                if (vecSymbol->type == SYMBOL_ID_BYTE) {
                    for (const auto& val : final_vals) {
                        fprintf(out, "\t.byte %s\n", val.c_str());
                    }
                } else if (vecSymbol->type == SYMBOL_ID_REAL) {
                    for (const auto& val : final_vals) {
                        // pra reais, divide a string "num/den" e coloca dois .long
                        size_t slash_pos = val.find('/');
                        string numerator = "0";
                        string denominator = "1";
                        if (slash_pos != string::npos) {
                            numerator = val.substr(0, slash_pos);
                            denominator = val.substr(slash_pos + 1);
                        } else {
                            // se não houver '/', assume que é um número inteiro (mas imprime um warning, pois estaria errado)
                            fprintf(stderr, "Warning: literal REAL '%s' possui um formato inesperado. Tratando como INT.\n", val.c_str());
                            numerator = val;
                        }
                        fprintf(out, "\t.long %s\n", numerator.c_str());
                        fprintf(out, "\t.long %s\n", denominator.c_str());
                    }
                } else { // default: INT
                    for (const auto& val : final_vals) {
                        fprintf(out, "\t.long %s\n", val.c_str());
                    }
                }

            } else { // vetor não inicializado
                int mult = 4; // default: INT
                if (vecSymbol->type == SYMBOL_ID_BYTE) mult = 1;
                else if (vecSymbol->type == SYMBOL_ID_REAL) mult = 8;
                
                int spaceInBytes = declaredSize * mult;
                fprintf(out, "\t.space %d\n", spaceInBytes);
            }
        }
    }

    // declara os formatos de impressão pro printf
    fprintf(out, "\n\t# Formatos de Impressao/Leitura\n");
    fprintf(out, ".LC_INT:\n");
    fprintf(out, "\t.string \"%%d \"\n"); // formato pra INT
    fprintf(out, ".LC_REAL:\n");
    fprintf(out, "\t.string \"%%d/%%d \"\n"); // formato pra REAL
    fprintf(out, ".LC_CHAR:\n");
    fprintf(out, "\t.string \"%%c \"\n"); // formato pra CHAR
    fprintf(out, ".LC_STRING:\n");
    fprintf(out, "\t.string \"%%s\"\n"); // formato pra STRING
    
    // formatos pra scanf (comando "read")
    fprintf(out, ".LC_SCAN_INT:\n");
    fprintf(out, "\t.string \"%%d\"\n"); // formato pra inteiro
    fprintf(out, ".LC_SCAN_REAL:\n");
    fprintf(out, "\t.string \"%%d/%%d\"\n"); // formato pra datatype real
    fprintf(out, ".LC_SCAN_STRING:\n");
    fprintf(out, "\t.string \"%%255s\"\n"); // formato pra ler uma string pro buffer
    fprintf(out, ".LC_SCAN_CHAR:\n");
    fprintf(out, "\t.string \" %%c\"\n"); // formato pra ler um char (tanto esse quanto o de string são pra converter caracteres pra ASCII)

    // buffer pra leitura de valores com datatype DATA_INT (usado na função _read_and_convert)
    fprintf(out, "\t.comm .read_buffer, 256, 32\n");

    // declarações de literais, feitos similarmente as variáveis
    for (auto const& lit : literalOrder) {
      const auto &label = literalLabels[lit];
      fprintf(out, "\t.globl %s\n", label.c_str());
      SYMBOL* s = symbolLookup(const_cast<char*>(lit.c_str()));

      // usa o dataType para escolher a diretiva assembly correta
      switch (s->dataType) {
          case DATA_INT:
              // verifica se o literal é do tipo char pra converter pra ASCII
              if (s->type == SYMBOL_CHAR) {
                  // extrai o caractere e obtém seu valor ASCII
                  fprintf(out, "\t.align 4\n");
                  fprintf(out, "%s:\n", label.c_str());
                  fprintf(out, "\t.long %d\n", (int)s->text[1]);
              } else {
                  // se for um inteiro normal, imprime o seu valor
                  fprintf(out, "\t.align 4\n");
                  fprintf(out, "%s:\n", label.c_str());
                  fprintf(out, "\t.long %s\n", s->text.c_str());
              }
              break;
          case DATA_REAL:
          {
              const std::string& real_lit = s->text;
              size_t slash_pos = real_lit.find('/');
              string numerator = "0";
              string denominator = "1";

              if (slash_pos != string::npos) {
                  numerator = real_lit.substr(0, slash_pos);
                  denominator = real_lit.substr(slash_pos + 1);
              } else {
                  // adiciona um aviso se o formato não for o esperado
                  fprintf(stderr, "Warning: literal REAL '%s' possui um formato inesperado. Considerando como 0/1.\n", real_lit.c_str());
              }

              fprintf(out, "\t.align 4\n");
              fprintf(out, "%s:\n", label.c_str());
              fprintf(out, "\t.long %s\n", numerator.c_str());
              fprintf(out, "\t.long %s\n", denominator.c_str());
              break;
          }
          case DATA_STRING:
              // usa .string pra literais de texto
              fprintf(out, "%s:\n", label.c_str());
              fprintf(out, "\t.string %s\n", s->text.c_str());
              break;
          default:
              fprintf(stderr, "Warning: Datatype do literal '%s' de tipo %d desconhecido. Tratando por padrão como string.\n", s->text.c_str(), s->dataType);
              fprintf(out, "%s:\n", label.c_str());
              fprintf(out, "\t.string %s\n", s->text.c_str());
              break;
      }
    }
}

// gera o código assembly pras instruções TAC
// a função recebe o primeiro nó da árvore de TACs e o arquivo de saída
// o código gerado é escrito no arquivo de saída em formato assembly AT&T
void asmGenerateCode(TAC* first, FILE* out) {
    fprintf(out, "\t.section .text\n");
    fprintf(out, "\t.globl main\n");

    for (TAC* tac = first; tac; tac = tac->next) {
        switch (tac->tipo) {
            case TAC_FUNC_START:
            {
                fprintf(out, "%s:\n", tac->resultado->text.c_str());
                fprintf(out, "\tpushq %%rbp\n");
                fprintf(out, "\tmovq %%rsp, %%rbp\n");

                // coleta parâmetros da lista de TACs (que vem após o TAC_FUNC_START)
                vector<SYMBOL*> params;
                TAC* p_node = tac->next;
                while (p_node && (p_node->tipo == TAC_PARAM_LIST || p_node->tipo == TAC_SYMBOL)) {
                    if (p_node->tipo == TAC_PARAM_LIST && p_node->resultado) {
                        params.push_back(p_node->resultado);
                    }
                    p_node = p_node->next;
                }

                std::reverse(params.begin(), params.end());

                // move os argumentos dos registradores pras variáveis globais correspondentes
                // seguindo a convenção de chamada x86-64 (máximo de 6 argumentos nos registradores)
                const char* arg_regs_32[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
                const char* arg_regs_8[] = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
                size_t arg_reg_idx = 0; // índice pra percorrer registradores

                for (size_t i = 0; i < params.size(); ++i) {
                    SYMBOL* p = params[i];
                    if (arg_reg_idx >= 6) {
                        fprintf(stderr, "ERRO! Número de argumentos excede o limite de 6 para registradores! Use menos argumentos em sua função.\n");
                        break;
                    }

                    fprintf(out, "\t# Movendo argumento %zu (%s) do registrador pra variavel global\n", i + 1, p->text.c_str());
                    
                    if (p->dataType == DATA_REAL) {
                        if (arg_reg_idx + 1 >= 6) {
                            fprintf(stderr, "ERRO! Parâmetro REAL excede o limite de registradores!\n");
                            break;
                        }
                        // um REAL consome dois registradores (formato numerador/denominador)
                        string addr = getSymbolAddress(p);
                        fprintf(out, "\tmovl\t%s, %s\n", arg_regs_32[arg_reg_idx], addr.c_str()); // numerador
                        fprintf(out, "\tmovl\t%s, 4+%s\n", arg_regs_32[arg_reg_idx + 1], addr.c_str()); // denominador
                        arg_reg_idx += 2;
                    } else if (p->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\tmovb\t%s, %s\n", arg_regs_8[arg_reg_idx], getSymbolAddress(p).c_str());
                        arg_reg_idx++;
                    } else { // default: int
                        fprintf(out, "\tmovl\t%s, %s\n", arg_regs_32[arg_reg_idx], getSymbolAddress(p).c_str());
                        arg_reg_idx++;
                    }
                }
                // avança o ponteiro de percorrer as tacs pra depois da lista de parâmetros
                if (p_node) {
                    tac = p_node->prev;
                }
                break;
            }
            case TAC_FUNC_END:
                // finaliza a função, restaurando o ponteiro de base e retornando
                fprintf(out, "\t# Fim da função %s\n", tac->resultado->text.c_str());
                fprintf(out, "\tleave\n");
                fprintf(out, "\tret\n");
                break;

            case TAC_CMD_ASSIGN:
                // verifica se a atribuição é do tipo REAL
                if (tac->op1 && tac->op1->dataType == DATA_REAL) {
                    fprintf(out, "\t# Atribuição REAL\n");
                    // move o numerador e o denominador (8 bytes)
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t4+%s, %%edx\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1 && (tac->op1->type == SYMBOL_ID_BYTE || tac->op1->type == SYMBOL_CHAR)) {
                    // atribuição de byte (apenas 1 byte)
                    fprintf(out, "\t# Atribuição BYTE\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // default: INT
                    fprintf(out, "\t# Atribuição INT\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;
            
            case TAC_LABEL:
                // imprime um label
                fprintf(out, "%s:\n", tac->resultado->text.c_str());
                break;

            case TAC_JUMP_FALSE:
                // se op1 for nulo, é um salto incondicional (jmp)
                if (tac->op1 == nullptr) {
                    fprintf(out, "\t# Salto incondicional pra %s\n", tac->resultado->text.c_str());
                    fprintf(out, "\tjmp\t%s\n", tac->resultado->text.c_str());
                } else {
                    fprintf(out, "\t# Salto condicional pra %s\n", tac->resultado->text.c_str());
                    // carrega o valor da condição (op1) pro registrador
                    fprintf(out, "\tmovzbl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    // compara o registrador com 0
                    fprintf(out, "\ttestl\t%%eax, %%eax\n");
                    // salta pro LABEL se o resultado for zero (false)
                    fprintf(out, "\tje\t%s\n", tac->resultado->text.c_str());
                }
                break;

            case TAC_JUMP_TRUE:
                // se op1 for nulo, é um salto incondicional (jmp)
                if (tac->op1 == nullptr) {
                    fprintf(out, "\t# Salto incondicional pra %s\n", tac->resultado->text.c_str());
                    fprintf(out, "\tjmp\t%s\n", tac->resultado->text.c_str());
                } else {
                    fprintf(out, "\t# Salto condicional pra %s\n", tac->resultado->text.c_str());
                    // carrega o valor da condição (op1) pro registrador
                    fprintf(out, "\tmovzbl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    // compara o registrador com 0
                    fprintf(out, "\ttestl\t%%eax, %%eax\n");
                    // salta pro LABEL se o resultado for não-zero (true)
                    fprintf(out, "\tjne\t%s\n", tac->resultado->text.c_str());
                }
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
                    // simplifica a fração após fazer a operação
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte ADD\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\taddb\t%%bl, %%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int ADD\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\taddl\t%%edx, %%eax\n");
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
                    // simplifica a fração após fazer a operação
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte SUB\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsubb\t%%bl, %%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int SUB\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tsubl\t%%edx, %%eax\n");
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
                    // simplifica a fração após fazer a operação
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte MUL\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\timulb\t%s\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int MUL\n");
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
                    // simplifica a fração após fazer a operação
                    fprintf(out, "\tmovq\t%%rax, %%rdi\n\tmovq\t%%rcx, %%rsi\n\tcall\t_simplify_fraction\n");
                    fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(tac->resultado).c_str());
                    fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte DIV\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str()); // Dividendo em %al
                    fprintf(out, "\tcbw\n"); // Estende o sinal de %al para %ax
                    fprintf(out, "\tidivb\t%s\n", getSymbolAddress(tac->op2).c_str()); // Divide %ax pelo operando
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str()); // Quociente está em %al
                } else { // operação com inteiros
                    fprintf(out, "\t# Int DIV\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovl\t%s, %%ecx\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\txorl\t%%edx, %%edx\n"); // Zera o registrador edx
                    fprintf(out, "\tcltd\n");
                    fprintf(out, "\tidivl\t%%ecx\n");
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
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte LESS\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tcmpb\t%%bl, %%al\n");
                    fprintf(out, "\tsetl\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int LESS\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str()); // carrega op1 em %eax
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str()); // carrega op2 em %edx
                    fprintf(out, "\tcmpl\t%%edx, %%eax\n"); // compara %eax com %edx
                    fprintf(out, "\tsetl\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
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
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte GREATER\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tcmpb\t%%bl, %%al\n");
                    fprintf(out, "\tsetg\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int GREATER\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str()); // carrega op1 em %eax
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str()); // carrega op2 em %edx
                    fprintf(out, "\tcmpl\t%%edx, %%eax\n"); // compara %eax com %edx
                    fprintf(out, "\tsetg\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
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
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte EQUAL\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tcmpb\t%%bl, %%al\n");
                    fprintf(out, "\tsete\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int EQUAL\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str()); // carrega op1 em %eax
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str()); // carrega op2 em %edx
                    fprintf(out, "\tcmpl\t%%edx, %%eax\n"); // compara %eax com %edx
                    fprintf(out, "\tsete\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
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
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte NEQUAL\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tcmpb\t%%bl, %%al\n");
                    fprintf(out, "\tsetne\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int NEQUAL\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str()); // carrega op1 em %eax
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str()); // carrega op2 em %edx
                    fprintf(out, "\tcmpl\t%%edx, %%eax\n"); // compara %eax com %edx
                    fprintf(out, "\tsetne\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
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
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte GEQ\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tcmpb\t%%bl, %%al\n");
                    fprintf(out, "\tsetge\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // operação com inteiros
                    fprintf(out, "\t# Int GEQ\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str()); // carrega op1 em %eax
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str()); // carrega op2 em %edx
                    fprintf(out, "\tcmpl\t%%edx, %%eax\n"); // compara %eax com %edx
                    fprintf(out, "\tsetge\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
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
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else if (tac->op1->type == SYMBOL_ID_BYTE || tac->op2->type == SYMBOL_ID_BYTE) {
                    fprintf(out, "\t# Byte LEQ\n");
                    fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                    fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                    fprintf(out, "\tcmpb\t%%bl, %%al\n");
                    fprintf(out, "\tsetle\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                } else { // Operação com inteiros
                    fprintf(out, "\t# Int LEQ\n");
                    fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->op1).c_str()); // carrega op1 em %eax
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(tac->op2).c_str()); // carrega op2 em %edx
                    fprintf(out, "\tcmpl\t%%edx, %%eax\n"); // compara %eax com %edx
                    fprintf(out, "\tsetle\t%%al\n");
                    fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                }
                break;

            case TAC_AND:
                fprintf(out, "\t# BOOL AND\n");
                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                fprintf(out, "\tandb\t%%bl, %%al\n");
                fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_OR:
                fprintf(out, "\t# BOOL OR\n");
                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tmovb\t%s, %%bl\n", getSymbolAddress(tac->op2).c_str());
                fprintf(out, "\torb\t%%bl, %%al\n");
                fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_NOT:
                fprintf(out, "\t# BOOL NOT\n");
                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->op1).c_str());
                fprintf(out, "\tnotb\t%%al\n");
                fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(tac->resultado).c_str());
                break;

            case TAC_VEC_READ:
                asmReadVector(tac, out);
                break;
            case TAC_PRINT_LIST:
            {
                // coleta todos os nós TAC_PRINT_LIST consecutivos
                vector<SYMBOL*> printed_values;
                TAC* node = tac;
                while (node && node->tipo == TAC_PRINT_LIST) {
                    if (node->resultado)
                        printed_values.push_back(node->resultado);
                    node = node->next;
                }
                // guarda o último PRINT_LIST processado
                TAC* last = node ? node->prev : tac;

                // inverte o vetor de valores pra imprimir na ordem correta
                for (size_t i = 0, j = printed_values.size() - 1; i < j; ++i, --j) {
                    SYMBOL* tmp = printed_values[i];
                    printed_values[i] = printed_values[j];
                    printed_values[j] = tmp;
                }

                // gera o código de print para cada item na ordem correta
                for (SYMBOL* item : printed_values) {
                    if (!item) continue;
                    fprintf(out, "\t# Imprimindo valor: %s\n", item->text.c_str());
                    // verifica o tipo do item de impressão e o imprime conforme seu tipo
                    if (item->type == SYMBOL_STRING) {
                        // usa printf com formato "%s" para não adicionar newline automaticamente
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

                // ajusta a tac pra pular todos os nós que acabamos de processar
                tac = last;
                break;
            }
            case TAC_CMD_PRINT:
                // ignora o comando print pois processa os TAC_PRINT_LIST ao invés
                break;
            case TAC_FUNCALL:
            {
                // coleta argumentos das TACs TAC_EXPR_LIST anteriores
                vector<SYMBOL*> args;
                TAC* current = tac->prev;
                while (current && current->tipo == TAC_EXPR_LIST) {
                    if (current->resultado) {
                        args.push_back(current->resultado);
                    }
                    current = current->prev;
                }

                // carrega argumentos nos registradores de acordo com a convenção x86-64.
                const char* arg_regs_32[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
                const char* arg_regs_8[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
                size_t arg_reg_idx = 0;

                fprintf(out, "\t# Preparando argumentos pra chamada de %s\n", tac->op1->text.c_str());
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
                        // passa o numerador no primeiro registrador e o denominador no segundo.
                        fprintf(out, "\tmovl\t%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx]);
                        fprintf(out, "\tmovl\t4+%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx + 1]);
                        arg_reg_idx += 2;
                    } else if (arg->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\tmovzbl\t%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_8[arg_reg_idx]);
                        arg_reg_idx++;
                    } else{ // default: INT
                        fprintf(out, "\tmovl\t%s, %s\n", getSymbolAddress(arg).c_str(), arg_regs_32[arg_reg_idx]);
                        arg_reg_idx++;
                    }
                }

                // chama a função, considerando que o nome da função está em op1
                fprintf(out, "\tcall\t%s\n", tac->op1->text.c_str());

                // obtém o valor de retorno e armazena ele no resultado (valor temporário)
                SYMBOL* funcSymbol = tac->op1;
                SYMBOL* resultSymbol = tac->resultado;

                if (resultSymbol) {
                    fprintf(out, "\t# Armazenando valor de retorno de %s em %s\n", funcSymbol->text.c_str(), resultSymbol->text.c_str());
                    // verifica o tipo de retorno da função pra saber de onde copiar o resultado
                    switch (funcSymbol->type) {
                        case SYMBOL_ID_REAL:
                            // reais (8 bytes) são retornados em %eax (numerador) e %edx (denominador)
                            fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(resultSymbol).c_str());
                            fprintf(out, "\tmovl\t%%edx, 4+%s\n", getSymbolAddress(resultSymbol).c_str());
                            break;

                        case SYMBOL_ID_BYTE:
                            // bytes (1 byte) são retornados na parte baixa de %rax
                            fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(resultSymbol).c_str());
                            break;

                        case SYMBOL_ID_INT:
                        default:
                            // inteiros (4 bytes) são retornados em %eax
                            fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(resultSymbol).c_str());
                            break;
                    }
                }
                break;
            }
            case TAC_CMD_RETURN:
            {
                if (tac->resultado) {
                    // verifica o tipo de dado do valor a ser retornado.
                    switch (tac->resultado->dataType) {
                        case DATA_REAL:
                            // carrega o numerador em %eax e o denominador em %edx.
                            fprintf(out, "\t# Retornando REAL\n");
                            fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->resultado).c_str());
                            fprintf(out, "\tmovl\t4+%s, %%edx\n", getSymbolAddress(tac->resultado).c_str());
                            break;

                        case DATA_VECTOR:
                            // tecnicamente esse caso não deveria acontecer, pois deveria ocorrer um TAC_VEC_READ antes
                            // colocando um valor temporário no resultado.
                            fprintf(out, "\t# Retornando endereço de vetor\n");
                            fprintf(out, "\tmovq\t%s, %%rax\n", getSymbolAddress(tac->resultado).c_str());
                            break;
                        case DATA_INT:
                        default:
                            // verifica se o tipo é BYTE pra usar movb.
                            if (tac->resultado->type == SYMBOL_ID_BYTE) {
                                fprintf(out, "\t# Retornando BYTE\n");
                                fprintf(out, "\tmovb\t%s, %%al\n", getSymbolAddress(tac->resultado).c_str());
                            } else{
                                // se não, trata como um INT
                                fprintf(out, "\t# Retornando INT\n");
                                fprintf(out, "\tmovl\t%s, %%eax\n", getSymbolAddress(tac->resultado).c_str());
                            }
                            break;
                    }
                }
                // após carregar o valor de retorno, sai da função
                fprintf(out, "\t# Saindo da função por meio do return\n");
                fprintf(out, "\tleave\n");
                fprintf(out, "\tret\n");
                break;
            }
            case TAC_CMD_VEC_ASSIGN:
            {
                // Atribuição de valor a um elemento do vetor
                // o vetor é acessado por um índice, e o valor é atribuído a esse índice
                SYMBOL* vecSymbol = tac->resultado;
                SYMBOL* indexSymbol = tac->op1;
                SYMBOL* valueSymbol = tac->op2;

                fprintf(out, "\t# Atribuindo a elemento de vetor: %s[%s] = %s\n", vecSymbol->text.c_str(), indexSymbol->text.c_str(), valueSymbol->text.c_str());

                // carrega o índice pra %rcx
                fprintf(out, "\tmovl\t%s, %%ecx\n", getSymbolAddress(indexSymbol).c_str());
                // carrega o endereço base do vetor pra %rax
                fprintf(out, "\tleaq\t%s(%%rip), %%rax\n", vecSymbol->text.c_str());


                // trata essa atribuição com base no tipo do vetor
                if (vecSymbol->type == SYMBOL_ID_REAL) {
                    // carrega o numerador e denominador do valor a ser atribuído
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(valueSymbol).c_str()); // Numerador
                    fprintf(out, "\tmovl\t4+%s, %%r8d\n", getSymbolAddress(valueSymbol).c_str()); // Denominador
                    // armazena o numerador na posição correta
                    fprintf(out, "\tmovl\t%%edx, (%%rax, %%rcx, 8)\n");
                    // armazena o denominador 4 bytes depois
                    fprintf(out, "\tmovl\t%%r8d, 4(%%rax, %%rcx, 8)\n");
                } else if (vecSymbol->type == SYMBOL_ID_BYTE) {
                    // carrega o valor (1 byte) pra parte baixa de %edx
                    fprintf(out, "\tmovb\t%s, %%dl\n", getSymbolAddress(valueSymbol).c_str());
                    // armazena o byte na posição correta
                    fprintf(out, "\tmovb\t%%dl, (%%rax, %%rcx, 1)\n");
                } else { // default: INT
                    // carrega o valor pra %edx
                    fprintf(out, "\tmovl\t%s, %%edx\n", getSymbolAddress(valueSymbol).c_str());
                    // armazena o inteiro na posição correta
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

                // convenção de chamada x86-64 pra scanf:
                // 1. %rdi <- endereço da string de formato
                // 2. %rsi, %rdx, ... <- endereços das variáveis pra armazenar os dados
                // 3. %eax <- 0

                if (var->dataType == DATA_REAL) {
                    // tipo REAL: formato "%d/%d", precisa de dois ponteiros de destino
                    // %rsi -> endereço do numerador
                    // %rdx -> endereço do denominador
                    fprintf(out, "\tleaq\t.LC_SCAN_REAL(%%rip), %%rdi\n");
                    fprintf(out, "\tleaq\t%s, %%rsi\n", getSymbolAddress(var).c_str());
                    fprintf(out, "\tleaq\t4+%s, %%rdx\n", getSymbolAddress(var).c_str());
                    fprintf(out, "\tmovl\t$0, %%eax\n");
                    fprintf(out, "\tcall\tscanf@PLT\n");
                } else {
                    // pro tipo INT e BYTE, chama a função auxiliar pra converter char se necessário
                    // o scanf é chamado nessa função
                    fprintf(out, "\tcall\t_read_and_convert\n");
                    // o resultado (valor inteiro) foi colocado em %eax
                    if (var->type == SYMBOL_ID_BYTE) {
                        fprintf(out, "\tmovb\t%%al, %s\n", getSymbolAddress(var).c_str());
                    } else {
                        fprintf(out, "\tmovl\t%%eax, %s\n", getSymbolAddress(var).c_str());
                    }
                }
                break;
            }
        default:
                //fprintf(stderr, "Geração de ASM não implementada pra TACs do tipo: %d\n", tac->tipo);
                break;
        }
    }

    // funções auxiliares em assembly
    // anotação: tive que pedir ajuda do chatGPT pra escrever essas funções auxiliares
    // especialmente a do máximo divisor comum (GCD) e simplificação de fração
    fprintf(out, "\n# --- Funcoes Auxiliares ---\n");

    // _read_and_convert: lê uma string, converte para int ou char (ASCII)
    // não recebe nada de entrada, mas usa o buffer global
    // saída: valor inteiro em %eax
    fprintf(out, "_read_and_convert:\n");
    fprintf(out, "\tpushq\t%%rbp\n");
    fprintf(out, "\tmovq\t%%rsp, %%rbp\n");
    // chama scanf para ler uma string pro buffer
    fprintf(out, "\tleaq\t.LC_SCAN_STRING(%%rip), %%rdi\n");
    fprintf(out, "\tleaq\t.read_buffer(%%rip), %%rsi\n");
    fprintf(out, "\tmovl\t$0, %%eax\n");
    fprintf(out, "\tcall\tscanf@PLT\n");
    // verifica o comprimento da string lida
    fprintf(out, "\tleaq\t.read_buffer(%%rip), %%rdi\n");
    fprintf(out, "\tcall\tstrlen@PLT\n");
    // compara o comprimento com 1
    fprintf(out, "\tcmpq\t$1, %%rax\n");
    fprintf(out, "\tjne\t_convert_atoi\n"); // se não for 1, assume que é um número e usa atoi
    // se o comprimento é 1, verifica se é um dígito
    fprintf(out, "\tmovzbl\t.read_buffer(%%rip), %%eax\n"); // pega o único caractere
    fprintf(out, "\tcmpb\t$48, %%al\n"); // compara com '0'
    fprintf(out, "\tjl\t_is_char\n"); // se for menor, é um caractere
    fprintf(out, "\tcmpb\t$57, %%al\n"); // compara com '9'
    fprintf(out, "\tjle\t_convert_atoi\n"); // se for menor ou igual, é um dígito, usa atoi
    fprintf(out, "_is_char:\n");
    fprintf(out, "\tmovzbl\t.read_buffer(%%rip), %%eax\n"); // é um caractere, então pega seu valor ASCII
    fprintf(out, "\tjmp\t_read_convert_end\n");
    fprintf(out, "_convert_atoi:\n");
    fprintf(out, "\tleaq\t.read_buffer(%%rip), %%rdi\n"); // é um número, usa atoi
    fprintf(out, "\tcall\tatoi@PLT\n");
    fprintf(out, "_read_convert_end:\n");
    fprintf(out, "\tleave\n");
    fprintf(out, "\tret\n\n");

    // _gcd: calcula o máximo divisor comum usando o algoritmo de euclides
    // faz o algoritmo mdc(a, b) = mdc(b, a % b) até que b seja 0
    // retorna o "a" como o mdc em %rax
    // entrada: %rdi, %rsi
    // saída: %rax
    fprintf(out, "_gcd:\n");
    fprintf(out, "\tmovq\t%%rdi, %%rax\n");
    fprintf(out, "\tmovq\t%%rsi, %%rbx\n");
    fprintf(out, "_gcd_loop:\n");
    fprintf(out, "\tcmpq\t$0, %%rbx\n");
    fprintf(out, "\tje\t_gcd_end\n");
    fprintf(out, "\tmovq\t%%rbx, %%rcx\n");
    fprintf(out, "\tmovq\t%%rax, %%rdx\n");
    fprintf(out, "\tmovq\t$0, %%rax\n"); // limpa rax pra divisão
    fprintf(out, "\tmovq\t%%rdx, %%rax\n");
    fprintf(out, "\tmovq\t$0, %%rdx\n"); // limpa rdx pro resto
    fprintf(out, "\tidivq\t%%rcx\n");
    fprintf(out, "\tmovq\t%%rcx, %%rax\n");
    fprintf(out, "\tmovq\t%%rdx, %%rbx\n");
    fprintf(out, "\tjmp\t_gcd_loop\n");
    fprintf(out, "_gcd_end:\n");
    fprintf(out, "\tret\n");

    // _simplify_fraction: simplifica uma fração, usando a função _gcd
    // entrada: numerador em %rdi, denominador em %rsi
    // saída: numerador simplificado em %rax, denominador simplificado em %rdx
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
    fprintf(out, "\tpushq\t%%rax\n"); // salva numerador simplificado
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
    
    // encontra o início da lista de TACs
    TAC* first = tac;
    while (first && first->prev) {
        first = first->prev;
    }

    // pré-processa pra encontrar todos os literais
    // antes de gerar a seção de dados
    for (TAC* t = first; t; t = t->next) {
        auto processSymbol = [&](SYMBOL* s) {
            if (s && (s->type == SYMBOL_INT || s->type == SYMBOL_REAL || s->type == SYMBOL_CHAR || s->type == SYMBOL_STRING)) {
                if (literalLabels.find(s->text) == literalLabels.end()) {
                    literalLabels[s->text] = "LIT" + to_string(nextLiteralLabel++);
                    literalOrder.push_back(s->text);
                }
            }
        };
        
        processSymbol(t->resultado);
        processSymbol(t->op1);
        processSymbol(t->op2);
    }

    // gera a seção de dados, passando pela lista de TACs
    asmGenerateDataSection(first, out);

    // gera a seção de código
    asmGenerateCode(first, out);
    fprintf(stderr, "Arquivo ASM gerado com sucesso!\n");
}