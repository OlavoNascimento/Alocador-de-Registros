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
    int cor;
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
    vertice->cor = -1;

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

int grafo_obter_id(Grafo grafo) {
    return grafo->id;
}

int grafo_obter_max_cores(Grafo grafo) {
    return grafo->max_cores;
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

// Remove os nós com menor grau quando algum nó tem grau < que o número de cores.
// Remove os nós com maior grau quando todos os nós tem grau >= que o número de cores.
Pilha simplificar(Grafo grafo) {
    Pilha memoria = pilha_criar((PilhaDestruirInfo *) vertice_destruir);
    Vertice removido = NULL;
    while (grafo_obter_tamanho(grafo) > 0) {
        removido = grafo_remover_grau_menor(grafo);
        if (removido != NULL) {
            printf("Push: %d\n", removido->registrador);
            pilha_inserir(memoria, removido);
        } else {
            removido = grafo_remover_grau_maior(grafo);
            if (removido != NULL) {
                printf("Push: %d *\n", removido->registrador);
                pilha_inserir(memoria, removido);
            }
        }
    }
    return memoria;
}

void definir_registradores(int n, Pilha memoria) {
    Pilha processados = pilha_criar((PilhaDestruirInfo *) vertice_destruir);
    // Vetor marcando as cores em uso.
    bool *cores_em_uso = calloc(sizeof *cores_em_uso, n);

    Vertice vert = NULL;
    while ((vert = pilha_remover(memoria)) != NULL) {
        pilha_inserir(processados, vert);

        // A cada vértice os registradores disponíveis são reiniciados.
        for (int i = 0; i < n; i++)
            cores_em_uso[i] = false;

        // Marca as cores utilizadas pelos registradores conflitantes.
        for_each_lista(no, vert->arestas) {
            Vertice conflito = lista_obter_info(no);
            if (conflito->cor != -1)
                cores_em_uso[conflito->cor] = true;
        }

        // Encontra a posição da primeira cor que não está em uso.
        int registrador_livre = -1;
        for (int i = 0; i < n; i++) {
            if (!cores_em_uso[i]) {
                registrador_livre = i;
                break;
            }
        }

        if (registrador_livre != -1) {
            vert->cor = registrador_livre;
            printf("Pop: %d -> %d\n", vert->registrador, vert->cor);
        } else {
            // Não há nenhuma cor disponível para o registrador atual.
            // Cancela a alocação de registradores com n cores.
            printf("Pop: %d -> NO COLOR AVAILABLE\n", vert->registrador);
            break;
        }
    }

    free(cores_em_uso);
    pilha_destruir(processados);
}

void alocar(Grafo grafo, int n) {
    printf("----------------------------------------\n");
    printf("K=%d\n\n", n);

    Pilha memoria = simplificar(grafo);
    definir_registradores(n, memoria);
    pilha_destruir(memoria);

    printf("----------------------------------------\n");
}
