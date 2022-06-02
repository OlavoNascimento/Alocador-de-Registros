#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#include "lista.h"

typedef struct Grafo_s *Grafo;
typedef struct Aresta_s *Aresta;
typedef struct Vertice_s *Vertice;

// Remover tamanho do construtor e aumentar o tamanho conforme necessário.
Grafo grafo_criar(int id, int max_cores);

Vertice grafo_inserir_vertice(Grafo grafo, int registrador);

void grafo_inserir_aresta(Grafo grafo, int virtual, int conflito);

void vertice_destruir(Vertice vertice);

void grafo_destruir(Grafo grafo);

void simplificar(Grafo grafo);

#endif
