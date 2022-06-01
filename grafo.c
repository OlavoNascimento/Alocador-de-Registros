#include "grafo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "./lista.h"
#include "./utils.h"

#define TAMANHO_INICIAL 16

struct Vertice_s {
    Lista arestas;
    int registrador;
};

struct Grafo_s {
    size_t tamanho_atual;
    size_t tamanho_maximo;
    Vertice *vertices;
    int max_cores;
    int id;
};

// Remover tamanho do construtor e aumentar o tamanho conforme necessário.
Grafo grafo_criar(int id, int max_cores) {
    Grafo grafo = malloc(sizeof *grafo);
    if (grafo == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um grafo!\n");
        return NULL;
    }
    grafo->tamanho_atual = 0;
    grafo->tamanho_maximo = TAMANHO_INICIAL;
    grafo->id = id;
    grafo->max_cores = max_cores;

    grafo->vertices = malloc(sizeof *grafo->vertices * grafo->tamanho_maximo);
    if (grafo->vertices == NULL) {
        free(grafo);
        LOG_ERRO("Falha ao alocar espaço para um grafo!\n");
        return NULL;
    }
    for (size_t i = 0; i < grafo->tamanho_maximo; i++)
        grafo->vertices[i] = NULL;
    LOG_INFO("Grafo criado, id: %d, cores: %d\n", id, max_cores);

    return grafo;
}

// Dobra o tamanho do array de vértices de um grafo.
bool aumentar_tamanho_grafo(Grafo grafo) {
    Vertice *temp = realloc(grafo->vertices, sizeof *temp * 2 * grafo->tamanho_maximo);
    if (temp == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return false;
    }

    grafo->vertices = temp;
    grafo->tamanho_maximo *= 2;
    for (size_t i = grafo->tamanho_atual; i < grafo->tamanho_maximo; i++)
        grafo->vertices[i] = NULL;

    return true;
}

Vertice grafo_inserir_vertice(Grafo grafo, int registrador) {
    if (grafo->tamanho_atual >= grafo->tamanho_maximo && !aumentar_tamanho_grafo(grafo)) {
        LOG_AVISO("Falha ao adicionar vértice a um grafo!\n");
        return NULL;
    }

    Vertice vertice = malloc(sizeof *vertice);
    if (vertice == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }
    vertice->registrador = registrador;

    vertice->arestas =
        lista_criar((ObterIdentificadorLista *) aresta_obter_destino, (ListaDestruirInfo *) free);
    if (vertice->arestas == NULL) {
        free(vertice);
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }

    const int index = grafo->tamanho_atual;
    grafo->vertices[index] = vertice;
    grafo->tamanho_atual++;
    LOG_INFO("Registrador físico %d inserido no grafo\n", registrador);

    return vertice;
}

void grafo_inserir_aresta(Grafo grafo, int fisico, int conflito) {
    if (grafo->tamanho_atual >= grafo->tamanho_maximo && !aumentar_tamanho_grafo(grafo)) {
        LOG_AVISO("Falha ao adicionar vértice a um grafo!\n");
        return;
    }

    int index = -1;
    for (size_t i = 0; i < grafo->tamanho_atual; i++) {
        if (grafo->vertices[i]->registrador == fisico)
            index = i;
    }
    if (index == -1) {
        LOG_ERRO("Registrador %d não foi encontrado nos vértices do grafo!\n", fisico);
        return;
    }

    int *aresta = malloc(sizeof *aresta);
    if (aresta == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return;
    }
    *aresta = conflito;

    lista_inserir_final(grafo->vertices[index]->arestas, aresta);
    LOG_INFO("Conflito %d do registrador físico %d inserido no grafo\n", *aresta, fisico);
}

Lista vertice_obter_arestas(Vertice vertice) {
    return vertice->arestas;
}

int aresta_obter_destino(int *aresta) {
    return *aresta;
}

void vertice_destruir(Vertice vertice) {
    if (vertice == NULL)
        return;
    lista_destruir(vertice->arestas);
    free(vertice);
}

void grafo_destruir(Grafo grafo) {
    if (grafo == NULL)
        return;
    LOG_INFO("Destruindo grafo!\n");
    for (size_t i = 0; i < grafo->tamanho_maximo; i++)
        vertice_destruir(grafo->vertices[i]);
    free(grafo->vertices);
    free(grafo);
}
