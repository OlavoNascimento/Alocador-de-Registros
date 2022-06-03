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
    printf("Graph %d -> Physical Registers: %d\n", grafo_obter_id(grafo),
           grafo_obter_max_cores(grafo));
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    bool *status = calloc(sizeof *status, grafo_obter_max_cores(grafo) + 1);
    for (int i = 0; i < grafo_obter_max_cores(grafo); i++)
        status[i] = true;

    for (int i = grafo_obter_max_cores(grafo); i > 1; i--) {
        status[i] = alocar(grafo, i);
        LOG_AVISO("Resultado para alocação com %d cores: %d!\n", i, status[i]);
    }
    printf("----------------------------------------\n");
    for (int i = grafo_obter_max_cores(grafo); i > 1; i--) {
        if (status[i])
            printf("Graph %d -> K = %d: Successful Allocation\n", grafo_obter_id(grafo), i);
        else {
            printf("Graph %d -> K = %d: SPILL\n", grafo_obter_id(grafo), i);
        }
    }
    grafo_destruir(grafo);
    free(status);
    return 0;
}
