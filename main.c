#include "parser.tab.h"
#include "pilha.h"
#include "utils.h"

extern Pilha conflitos;

int main() {
    conflitos = pilha_criar(NULL);
    return yyparse();
}
