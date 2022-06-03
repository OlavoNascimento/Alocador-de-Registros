#include "grafo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "pilha.h"
#include "utils.h"

typedef bool Comparador(int a, int b);

struct Aresta_s {
    Vertice origem;
    Vertice destino;
};

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

int aresta_obter_destino(Aresta aresta) {
    return aresta->destino->registrador;
}

Vertice grafo_criar_vertice(int registrador) {
    Vertice vertice = malloc(sizeof *vertice);
    if (vertice == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }
    vertice->registrador = registrador;
    vertice->cor = -1;

    vertice->arestas = lista_criar((ObterIdentificadorLista *) aresta_obter_destino, free);
    if (vertice->arestas == NULL) {
        free(vertice);
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }
    return vertice;
}

void grafo_inserir_vertice(Grafo grafo, Vertice vert) {
    lista_inserir_final(grafo->vertices, vert);
    LOG_INFO("Registrador físico %d com grau %d inserido no grafo\n", vert->registrador,
             lista_obter_tamanho(vert->arestas));
}

Aresta grafo_criar_aresta(Grafo grafo, int origem, int destino) {
    ListaInfo busca_origem = lista_buscar(grafo->vertices, origem);
    if (busca_origem == NULL) {
        LOG_ERRO("Registrador de origem %d não foi encontrado nos vértices do grafo!\n", origem);
        return NULL;
    }

    ListaInfo busca_destino = lista_buscar(grafo->vertices, destino);
    if (busca_destino == NULL) {
        LOG_ERRO("Registrador de destino %d não foi encontrado nos vértices do grafo!\n", destino);
        return NULL;
    }
    Aresta aresta = malloc(sizeof *aresta);
    if (aresta == NULL) {
        LOG_ERRO("Falha ao alocar memória!\n");
        return NULL;
    }
    aresta->origem = lista_obter_info(busca_origem);
    aresta->destino = lista_obter_info(busca_destino);
    return aresta;
}

void grafo_inserir_aresta(Aresta aresta) {
    lista_inserir_final(aresta->origem->arestas, aresta);
    LOG_INFO("Conflito %d -> %d inserido no grafo, vértice %d agora tem grau %d\n",
             aresta->origem->registrador, aresta->destino->registrador, aresta->origem->registrador,
             lista_obter_tamanho(aresta->origem->arestas));
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

void vertice_diminuir_grau_arestas(Vertice vert, Pilha arestas_removidas) {
    for_each_lista(no, vert->arestas) {
        Aresta aresta = lista_obter_info(no);
        ListaNo conexao = lista_buscar(aresta->destino->arestas, aresta->origem->registrador);
        LOG_INFO("Diminuindo grau %d -> %d\n", aresta->origem->registrador,
                 aresta->destino->registrador);

        Aresta removido = lista_remover(aresta->destino->arestas, conexao);
        // Faz uma cópia da aresta para recriar o grafo quando essa execução com n cores terminar.
        pilha_inserir(arestas_removidas, removido);
    }
}

Vertice grafo_remover_grau_menor(Grafo grafo, int max_cores) {
    Vertice antigo = NULL;
    int grau_comp = 0;
    ListaNo no_comp = NULL;

    for_each_lista(no, grafo->vertices) {
        const Vertice vert = lista_obter_info(no);
        const int grau = lista_obter_tamanho(vert->arestas);

        if (grau < max_cores) {
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

    return lista_remover(grafo->vertices, no_comp);
}

Vertice grafo_remover_grau_maior(Grafo grafo, int max_cores) {
    Vertice antigo = NULL;
    int grau_comp = 0;
    ListaNo no_comp = NULL;

    for_each_lista(no, grafo->vertices) {
        const Vertice vert = lista_obter_info(no);
        const int grau = lista_obter_tamanho(vert->arestas);

        if (grau >= max_cores) {
            if (grau == grau_comp && antigo != NULL && vert->registrador < antigo->registrador) {
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

    return lista_remover(grafo->vertices, no_comp);
}

int grafo_obter_tamanho(Grafo grafo) {
    return lista_obter_tamanho(grafo->vertices);
}

// Remove os nós com menor grau quando algum nó tem grau < que o número de cores.
// Remove os nós com maior grau quando todos os nós tem grau >= que o número de cores.
Pilha simplificar(Grafo grafo, int max_cores, Pilha vertices_removidos, Pilha arestas_removidas) {
    Pilha memoria = pilha_criar(NULL);
    Vertice removido = NULL;
    while (grafo_obter_tamanho(grafo) > 0) {
        removido = grafo_remover_grau_menor(grafo, max_cores);
        if (removido != NULL) {
            printf("Push: %d\n", removido->registrador);
            pilha_inserir(memoria, removido);
            vertice_diminuir_grau_arestas(removido, arestas_removidas);
            // Salva o vértices para recriar o grafo após esse execução de alocação com n cores.
            pilha_inserir(vertices_removidos, removido);
        } else {
            removido = grafo_remover_grau_maior(grafo, max_cores);
            if (removido != NULL) {
                printf("Push: %d *\n", removido->registrador);
                pilha_inserir(memoria, removido);
                vertice_diminuir_grau_arestas(removido, arestas_removidas);
                // Salva o vértices para recriar o grafo após esse execução de alocação com n cores.
                pilha_inserir(vertices_removidos, removido);
            }
        }
    }
    return memoria;
}

bool definir_registradores(int max_cores, Pilha memoria) {
    bool status = true;
    // Vetor marcando as cores em uso.
    bool *cores_em_uso = calloc(sizeof *cores_em_uso, max_cores);

    Vertice vert = NULL;
    while ((vert = pilha_remover(memoria)) != NULL) {
        // A cada vértice os registradores disponíveis são reiniciados.
        for (int i = 0; i < max_cores; i++)
            cores_em_uso[i] = false;

        // Marca as cores utilizadas pelos registradores conflitantes.
        for_each_lista(no, vert->arestas) {
            Aresta aresta = lista_obter_info(no);
            if (aresta->destino->cor != -1)
                cores_em_uso[aresta->destino->cor] = true;
        }

        // Encontra a posição da primeira cor que não está em uso.
        int registrador_livre = -1;
        for (int i = 0; i < max_cores; i++) {
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
            status = false;
            break;
        }
    }

    free(cores_em_uso);
    return status;
}

void reconstruir_grafo(Grafo grafo, Pilha vertices_removidos, Pilha aresta_removidas) {
    Vertice vert = NULL;
    while ((vert = pilha_remover(vertices_removidos)) != NULL) {
        grafo_inserir_vertice(grafo, vert);
        vert->cor = -1;
    }

    Aresta aresta = NULL;
    while ((aresta = pilha_remover(aresta_removidas)) != NULL) {
        grafo_inserir_aresta(aresta);
    }
}

bool alocar(Grafo grafo, int max_cores) {
    bool status = true;
    printf("K = %d\n\n", max_cores);

    Pilha vertices_removidos = pilha_criar(NULL);
    Pilha arestas_removidas = pilha_criar(NULL);

    Pilha memoria = simplificar(grafo, max_cores, vertices_removidos, arestas_removidas);
    status = definir_registradores(max_cores, memoria);

    printf("----------------------------------------\n");

    reconstruir_grafo(grafo, vertices_removidos, arestas_removidas);

    pilha_destruir(memoria);
    pilha_destruir(arestas_removidas);
    pilha_destruir(vertices_removidos);
    return status;
}
