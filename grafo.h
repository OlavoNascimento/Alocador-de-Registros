#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#include "lista.h"

typedef struct Grafo_s *Grafo;
typedef struct Aresta_s *Aresta;
typedef struct Vertice_s *Vertice;

Grafo grafo_criar(int id, int max_cores);

Vertice grafo_inserir_vertice(Grafo grafo, int registrador);

void grafo_inserir_aresta(Grafo grafo, int virtual, int conflito);

int grafo_obter_id(Grafo grafo);

int grafo_obter_max_cores(Grafo grafo);

void vertice_destruir(Vertice vertice);

void grafo_destruir(Grafo grafo);

void alocar(Grafo grafo, int n);

#endif
