#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#include "lista.h"

typedef struct Grafo_s *Grafo;
typedef struct Aresta_s *Aresta;
typedef struct Vertice_s *Vertice;

Grafo grafo_criar(int id, int max_cores);

Vertice grafo_criar_vertice(int registrador);

void grafo_inserir_vertice(Grafo grafo, Vertice vert);

Aresta grafo_criar_aresta(Grafo grafo, int origem, int destino);

void grafo_inserir_aresta(Aresta aresta);

int grafo_obter_id(Grafo grafo);

int grafo_obter_max_cores(Grafo grafo);

void vertice_destruir(Vertice vertice);

void grafo_destruir(Grafo grafo);

bool alocar(Grafo grafo, int n);

#endif
