#include "grafo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "pilha.h"
#include "utils.h"

typedef bool Comparador(int a, int b);

struct Vertice_s {
    Lista arestas;
    int registrador;
};

struct Grafo_s {
    Lista vertices;
    int max_cores;
    int id;
};

int vertice_obter_registrador(Vertice vertice) {
    return vertice->registrador;
}

// Remover tamanho do construtor e aumentar o tamanho conforme necessário.
Grafo grafo_criar(int id, int max_cores) {
    Grafo grafo = malloc(sizeof *grafo);
    if (grafo == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um grafo!\n");
        return NULL;
    }
    grafo->id = id;
    grafo->max_cores = max_cores;
    grafo->vertices = lista_criar((ObterIdentificadorLista *) vertice_obter_registrador,
                                  (ListaDestruirInfo *) vertice_destruir);

    LOG_INFO("Grafo criado, id: %d, cores: %d\n", id, max_cores);
    return grafo;
}

Vertice grafo_inserir_vertice(Grafo grafo, int registrador) {
    Vertice vertice = malloc(sizeof *vertice);
    if (vertice == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }
    vertice->registrador = registrador;

    vertice->arestas = lista_criar((ObterIdentificadorLista *) vertice_obter_registrador, NULL);
    if (vertice->arestas == NULL) {
        free(vertice);
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }

    lista_inserir_final(grafo->vertices, vertice);
    LOG_INFO("Registrador físico %d inserido no grafo\n", registrador);

    return vertice;
}

void grafo_inserir_aresta(Grafo grafo, int virtual, int conflito) {
    ListaInfo busca_origem = lista_buscar(grafo->vertices, virtual);
    if (busca_origem == NULL) {
        LOG_ERRO("Registrador de origem %d não foi encontrado nos vértices do grafo!\n", virtual);
        return;
    }
    Vertice vert_origem = lista_obter_info(busca_origem);

    ListaInfo busca_destino = lista_buscar(grafo->vertices, conflito);
    if (busca_destino == NULL) {
        LOG_ERRO("Registrador de destino %d não foi encontrado nos vértices do grafo!\n", conflito);
        return;
    }
    Vertice vert_destino = lista_obter_info(busca_destino);

    lista_inserir_final(vert_origem->arestas, vert_destino);
    LOG_INFO("Conflito %d do registrador físico %d inserido no grafo\n", conflito, virtual);
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
    lista_destruir(grafo->vertices);
    free(grafo);
}

void vertice_diminuir_grau_arestas(Vertice vert) {
    if (vert == NULL)
        return;
    for_each_lista(no, vert->arestas) {
        Vertice destino = lista_obter_info(no);
        ListaNo conexao = lista_buscar(destino->arestas, vert->registrador);
        lista_remover(destino->arestas, conexao);
    }
}

Vertice grafo_remover_grau_menor(Grafo grafo) {
    Vertice antigo = NULL;
    int grau_comp = 0;
    ListaNo no_comp = NULL;

    for_each_lista(no, grafo->vertices) {
        const Vertice vert = lista_obter_info(no);
        const int grau = lista_obter_tamanho(vert->arestas);

        if (grau < grafo->max_cores) {
            if (grau == grau_comp && antigo != NULL && vert->registrador < antigo->registrador) {
                // Graus iguais, o menor registrador virtual é selecionado.
                antigo = vert;
                no_comp = no;
                grau_comp = lista_obter_tamanho(antigo->arestas);
            } else if (grau < grau_comp || antigo == NULL) {
                antigo = vert;
                no_comp = no;
                grau_comp = lista_obter_tamanho(antigo->arestas);
            }
        }
    }

    vertice_diminuir_grau_arestas(antigo);
    return lista_remover(grafo->vertices, no_comp);
}

Vertice grafo_remover_grau_maior(Grafo grafo) {
    Vertice antigo = NULL;
    int grau_comp = 0;
    ListaNo no_comp = NULL;

    for_each_lista(no, grafo->vertices) {
        const Vertice vert = lista_obter_info(no);
        const int grau = lista_obter_tamanho(vert->arestas);

        if (grau >= grafo->max_cores) {
            if (grau == grau_comp && vert->registrador < antigo->registrador) {
                // Graus iguais, o menor registrador virtual é selecionado.
                antigo = vert;
                no_comp = no;
                grau_comp = lista_obter_tamanho(antigo->arestas);
            } else if (grau > grau_comp || antigo == NULL) {
                antigo = vert;
                no_comp = no;
                grau_comp = lista_obter_tamanho(antigo->arestas);
            }
        }
    }

    vertice_diminuir_grau_arestas(antigo);
    return lista_remover(grafo->vertices, no_comp);
}

int grafo_obter_tamanho(Grafo grafo) {
    return lista_obter_tamanho(grafo->vertices);
}

void simplificar(Grafo grafo) {
    int n = grafo->max_cores;
    printf("Graph %d -> Physical Registers: %d\n", grafo->id, grafo->max_cores);
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    printf("K=%d\n\n", n);

    Pilha memoria = pilha_criar((PilhaDestruirInfo *) vertice_destruir);

    Vertice removido = NULL;
    while ((removido = grafo_remover_grau_menor(grafo)) != NULL) {
        printf("Push: %d\n", removido->registrador);
        pilha_inserir(memoria, removido);
    }
    while ((removido = grafo_remover_grau_maior(grafo)) != NULL) {
        printf("Push: %d *\n", removido->registrador);
        pilha_inserir(memoria, removido);
    }

    // int **registradores = calloc(sizeof *registradores, grafo->max_cores);

    // Vertice vert = NULL;
    // while ((vert = pilha_remover(memoria)) != NULL) {
    //     int **conflitos = calloc(sizeof *registradores, grafo->max_cores);

    //     for_each_lista(no, vert->arestas) {}
    // }

    pilha_destruir(memoria);
}
