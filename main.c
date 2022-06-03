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
    printf("Graph %d -> Physical Registers: %d\n", grafo_obter_id(grafo),
           grafo_obter_max_cores(grafo));
    printf("----------------------------------------\n");
    for (int i = grafo_obter_max_cores(grafo); i > 1; i--) {
        alocar(grafo, i);
    }

    grafo_destruir(grafo);
    return 0;
}
