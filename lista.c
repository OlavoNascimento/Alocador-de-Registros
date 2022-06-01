#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils.h"

struct ListaNo_s {
    ListaInfo info;
    struct ListaNo_s *proximo;
    struct ListaNo_s *anterior;
};

struct Lista_s {
    ObterIdentificadorLista *obter_identificador_info;
    ListaDestruirInfo *destruir_info;
    int tamanho;
    struct ListaNo_s *primeiro;
    struct ListaNo_s *ultimo;
};

Lista lista_criar(ObterIdentificadorLista obter_identificador_info,
                  ListaDestruirInfo destruir_info) {
    Lista lista = malloc(sizeof *lista);
    if (lista == NULL) {
        LOG_ERRO("Falha ao alocar espaço para uma nova lista!\n");
        return NULL;
    }
    lista->obter_identificador_info = obter_identificador_info;
    lista->destruir_info = destruir_info;
    lista->tamanho = 0;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    return lista;
}

ListaNo lista_inserir_final(Lista lista, const ListaInfo info) {
    if (info == NULL) {
        LOG_AVISO("Informação nula passada para lista_inserir_final!\n");
        return NULL;
    }

    ListaNo novo_no = malloc(sizeof *novo_no);
    if (novo_no == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um novo elemento de uma lista!\n");
        return NULL;
    }
    novo_no->info = info;
    novo_no->proximo = NULL;

    if (lista->primeiro == NULL) {
        lista->primeiro = novo_no;
        novo_no->anterior = NULL;
    } else {
        lista->ultimo->proximo = novo_no;
        novo_no->anterior = lista->ultimo;
    }

    lista->ultimo = novo_no;
    lista->tamanho++;
    return novo_no;
}

ListaNo lista_inserir_antes(Lista lista, ListaInfo info, ListaNo no) {
    if (no == NULL) {
        LOG_AVISO("Nó nulo passado para lista_inserir_antes!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_AVISO("Informação nula passada para lista_inserir_antes!\n");
        return NULL;
    }

    ListaNo no_anterior = NULL;
    ListaNo novo_no = malloc(sizeof *novo_no);
    if (novo_no == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um novo elemento de uma lista!\n");
        return NULL;
    }

    novo_no->info = info;
    if (no == lista->primeiro) {  // caso seja inserido antes do primeiro
        no->anterior = novo_no;
        novo_no->proximo = no;
        novo_no->anterior = NULL;

        lista->primeiro = novo_no;
    } else {
        no_anterior = no->anterior;
        no->anterior = novo_no;
        no_anterior->proximo = novo_no;

        novo_no->proximo = no;
        novo_no->anterior = no_anterior;
    }
    lista->tamanho++;

    return novo_no;
}

ListaNo lista_inserir_depois(Lista lista, ListaInfo info, ListaNo no) {
    if (no == NULL) {
        LOG_AVISO("Nó nulo passado para lista_inserir_depois!\n");
        return NULL;
    }
    if (info == NULL) {
        LOG_AVISO("Informação nula passada para lista_inserir_depois!\n");
        return NULL;
    }

    ListaNo no_proximo = NULL;
    ListaNo novo_no = malloc(sizeof *novo_no);
    if (novo_no == NULL) {
        LOG_ERRO("Falha ao alocar espaço para um novo elemento de uma lista!\n");
        return NULL;
    }

    novo_no->info = info;

    if (no == lista->ultimo) {  // Caso seja inserido depois do último.
        no->proximo = novo_no;
        lista->ultimo = novo_no;

        novo_no->anterior = no;
        novo_no->proximo = NULL;
    } else {
        no_proximo = no->proximo;
        no->proximo = novo_no;
        no_proximo->anterior = novo_no;

        novo_no->proximo = no_proximo;
        novo_no->anterior = no;
    }
    lista->tamanho++;

    return novo_no;
}

ListaInfo lista_remover(Lista lista, ListaNo no) {
    ListaNo no_anterior = no->anterior;
    ListaNo no_proximo = no->proximo;

    if (no == lista->primeiro) {  // Se for o primeiro elemento da lista.
        lista->primeiro = no_proximo;
        // Caso a lista só tenha um elemento não é necessário mudar o ponteiro do próximo elemento.
        if (no_proximo != NULL)
            no_proximo->anterior = NULL;
    } else if (no == lista->ultimo) {  // Se for o último elemento da lista.
        lista->ultimo = no_anterior;
        no_anterior->proximo = NULL;
    } else {  // Se estiver no meio da lista.
        no_anterior->proximo = no->proximo;
        no_proximo->anterior = no->anterior;
    }

    ListaInfo info = no->info;
    free(no);
    lista->tamanho--;
    return info;
}

ListaNo lista_buscar(Lista lista, const char *id) {
    if (lista->obter_identificador_info == NULL) {
        LOG_AVISO(
            "Não é possível buscar em uma lista que não possui a função obter_identificador_info "
            "definida!\n");
        return NULL;
    }

    for_each_lista(no, lista) {
        const char *id_atual = lista->obter_identificador_info(no->info);
        if (strcmp(id_atual, id) == 0)
            return no;
    }
    return NULL;
}

// Troca as informações armazenadas em dois nós.
void lista_trocar_info(ListaNo no1, ListaNo no2) {
    ListaInfo temp = lista_obter_info(no1);
    no1->info = lista_obter_info(no2);
    no2->info = temp;
}

// Aplica uma função a todas as informações de uma lista.
void lista_map(Lista lis, MapInfoLista f, void *extra) {
    for_each_lista(no, lis) {
        f(lista_obter_info(no), extra);
    }
}

int lista_obter_tamanho(Lista lista) {
    return lista->tamanho;
}

ListaNo lista_obter_inicio(Lista lista) {
    return lista->primeiro;
}

ListaNo lista_obter_fim(Lista lista) {
    return lista->ultimo;
}

ListaInfo lista_obter_info(ListaNo no) {
    return no->info;
}

ListaNo lista_obter_proximo(ListaNo no) {
    return no->proximo;
}

ListaNo lista_obter_anterior(ListaNo no) {
    return no->anterior;
}

void lista_destruir(Lista lista) {
    ListaNo no_atual = lista->primeiro;
    while (no_atual != NULL) {
        ListaNo no_proximo = no_atual->proximo;
        if (lista->destruir_info != NULL)
            lista->destruir_info(no_atual->info);
        free(no_atual);
        no_atual = no_proximo;
    }
    free(lista);
}
