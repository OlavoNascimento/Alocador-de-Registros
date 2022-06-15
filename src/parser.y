%{
    #include <stdlib.h>

    #include "parser.tab.h"
    #include "grafo.h"
    #include "pilha.h"
    #include "utils.h"

    int yylex();
    void yyerror(const char *);

    Grafo grafo = NULL;
    Pilha conflitos = NULL;
    Lista arestas = NULL;

    struct ParserAresta {
        int origem;
        int destino;
    };
%}

%define parse.error verbose

%union {
    int numero;
}

%token <numero> NUMERO
%token GRAFO
%token IGUAL
%token K
%token NOVA_LINHA
%token SETA
%token VIRGULA

%type <numero> max_registradores
%type <numero> nome

%start grafo

%%
    grafo: cabecalho corpo {
        for_each_lista(no, arestas) {
            struct ParserAresta *arestaInfo = lista_obter_info(no);
            // Como todos os vértices estão presentes no grafo, as arestas são inseridas.
            Aresta aresta = grafo_criar_aresta(grafo, arestaInfo->origem, arestaInfo->destino);
            if(aresta != NULL)
                grafo_inserir_aresta(aresta);
            free(arestaInfo);
        }

        pilha_destruir(conflitos);
        lista_destruir(arestas);
        conflitos = NULL;
        arestas = NULL;
    };

    cabecalho: nome NOVA_LINHA max_registradores NOVA_LINHA {
        grafo = grafo_criar($1, $3);
    };

    nome: GRAFO NUMERO VIRGULA {
        $$ = $2;
    };

    max_registradores: K IGUAL NUMERO {
        $$ = $3;
    };

    corpo:conflito
        | conflito NOVA_LINHA
        | conflito NOVA_LINHA corpo
    ;

    conflito: NUMERO SETA registradores {
        grafo_inserir_vertice(grafo, grafo_criar_vertice($1));

        int *conflito = NULL;
        while ((conflito = pilha_remover(conflitos)) != NULL) {
            // Armazena a aresta para ser inserida após todos os vértices serem lidos.
            struct ParserAresta *aresta = malloc(sizeof *aresta);
            aresta->origem = $1;
            aresta->destino = *conflito;
            lista_inserir_final(arestas, aresta);
            free(conflito);
       }
    };

    registradores: NUMERO {
        int *conflito = malloc(sizeof *conflito);
        *conflito = $1;
        pilha_inserir(conflitos, conflito);
    } | NUMERO registradores {
        int *conflito = malloc(sizeof *conflito);
        *conflito = $1;
        pilha_inserir(conflitos, conflito);
    };

%%

void yyerror (const char *msg) {
    grafo_destruir(grafo);
    pilha_destruir(conflitos);
    printf("Erro: %s\n", msg);
    exit(1);
}
