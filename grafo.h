#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#include "lista.h"

typedef struct Grafo_s *Grafo;
// typedef struct Aresta_s *Aresta;
typedef struct Vertice_s *Vertice;

// Remover tamanho do construtor e aumentar o tamanho conforme necessário.
Grafo grafo_criar(int id, int max_cores);

Vertice grafo_inserir_vertice(Grafo grafo, int registrador);

Lista vertice_obter_arestas(Vertice vertice);

void grafo_inserir_aresta(Grafo grafo, int fisico, int conflito);

int aresta_obter_destino(int *aresta);

void vertice_destruir(Vertice vertice);

void grafo_destruir(Grafo grafo);

#endif
