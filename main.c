#include <stdlib.h>

#include "grafo.h"
#include "lista.h"
#include "parser.tab.h"
#include "pilha.h"
#include "utils.h"

extern Pilha conflitos;
extern Lista arestas;
extern Grafo grafo;

void construir() {
    conflitos = pilha_criar(NULL);
    arestas = lista_criar(NULL, NULL);
    yyparse();
}

int main() {
    construir();
    const int max_cores = grafo_obter_max_cores(grafo);
    printf("Graph %d -> Physical Registers: %d\n", grafo_obter_id(grafo), max_cores);
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    bool *status = calloc(sizeof *status, max_cores + 1);
    for (int i = 0; i < max_cores; i++)
        status[i] = true;

    for (int i = max_cores; i > 1; i--) {
        status[i] = alocar(grafo, i);
        LOG_AVISO("Resultado para alocação com %d cores: %d!\n", i, status[i]);
    }
    printf("----------------------------------------\n");
    for (int i = max_cores; i > 1; i--) {
        if (i != max_cores)
            printf("\n");
        if (status[i])
            printf("Graph %d -> K = %*d: Successful Allocation", grafo_obter_id(grafo),
                   max_cores / 10, i);
        else {
            printf("Graph %d -> K = %*d: SPILL", grafo_obter_id(grafo), max_cores / 10, i);
        }
    }
    grafo_destruir(grafo);
    free(status);
    return 0;
}
