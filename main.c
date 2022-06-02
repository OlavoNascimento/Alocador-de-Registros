#include "grafo.h"
#include "parser.tab.h"
#include "pilha.h"
#include "utils.h"

extern Pilha conflitos;
extern Pilha arestas;
extern Grafo grafo;

void construir() {
    conflitos = pilha_criar(NULL);
    arestas = pilha_criar(NULL);
    yyparse();
}

int main() {
    construir();
    simplificar(grafo);

    grafo_destruir(grafo);
    return 0;
}
