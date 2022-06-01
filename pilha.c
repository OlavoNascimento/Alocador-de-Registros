#include "pilha.h"

#include <stdbool.h>
#include <stdlib.h>

#include "./utils.h"

struct No {
    PilhaInfo info;
    struct No *proximo;
};

struct Pilha_s {
    PilhaDestruirInfo *destruir_info;
    int tamanho;
    struct No *topo;
};

Pilha pilha_criar(PilhaDestruirInfo destruir_info) {
    Pilha pilha = malloc(sizeof *pilha);
    if (pilha == NULL) {
        LOG_ERRO("Falha ao alocar espaço para uma nova pilha!\n");
        return NULL;
    }
    pilha->tamanho = 0;
    pilha->topo = NULL;
    pilha->destruir_info = destruir_info;
    return pilha;
}

bool pilha_esta_vazia(Pilha pilha) {
    return pilha->topo == NULL;
}

int pilha_obter_tamanho(Pilha pilha) {
    return pilha->tamanho;
}

void pilha_inserir(Pilha pilha, PilhaInfo info) {
    struct No *novo_no = malloc(sizeof *novo_no);
    if (novo_no == NULL) {
        LOG_ERRO("Falha ao alocar memória para novo elemento da pilha!\n");
        return;
    }
    novo_no->info = info;
    novo_no->proximo = pilha->topo;
    pilha->topo = novo_no;
    pilha->tamanho++;
}

PilhaInfo pilha_remover(Pilha pilha) {
    if (pilha_esta_vazia(pilha))
        return NULL;

    PilhaInfo info = pilha->topo->info;
    struct No *topo = pilha->topo;
    pilha->topo = topo->proximo;
    free(topo);
    pilha->tamanho--;
    return info;
}

PilhaInfo pilha_obter_topo(Pilha pilha) {
    if (pilha_esta_vazia(pilha))
        return NULL;
    return pilha->topo->info;
}

void pilha_destruir(Pilha pilha) {
    struct No *no_atual = pilha->topo;
    while (no_atual != NULL) {
        struct No *no_proximo = no_atual->proximo;
        if (pilha->destruir_info != NULL)
            pilha->destruir_info(no_atual->info);
        free(no_atual);
        no_atual = no_proximo;
    }
    free(pilha);
}
