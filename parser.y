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
    Pilha arestas = NULL;

    struct Aresta {
        int origem;
        int destino;
    };
%}

%define parse.error verbose

%union {
    int number;
}

%token <number> NUMBER
%token ARROW
%token ASSIGN
%token COLON
%token GRAPH
%token K
%token NEW_LINE

%type <number> max_registers
%type <number> name

%start graph

%%
    graph: header body {
        struct Aresta *aresta = NULL;
        while ((aresta = pilha_remover(arestas)) != NULL) {
            // Como todos os vértices estão presentes no grafo, as arestas são inseridas.
            grafo_inserir_aresta(grafo, aresta->origem, aresta->destino);
            free(aresta);
        }

        pilha_destruir(conflitos);
        pilha_destruir(arestas);
        conflitos = NULL;
        arestas = NULL;
    };

    header: name NEW_LINE max_registers NEW_LINE {
        grafo = grafo_criar($1, $3);
    };

    name: GRAPH NUMBER COLON {
        $$ = $2;
    };

    max_registers: K ASSIGN NUMBER {
        $$ = $3;
    };

    body: conflict
        | conflict NEW_LINE
        | conflict NEW_LINE body
    ;

    conflict: NUMBER ARROW registers {
        grafo_inserir_vertice(grafo, $1);

        int *conflito = NULL;
        while ((conflito = pilha_remover(conflitos)) != NULL) {
            // Armazena a aresta para ser inserida após todos os vértices serem lidos.
            struct Aresta *aresta = malloc(sizeof *aresta);
            aresta->origem = $1;
            aresta->destino= *conflito;
            pilha_inserir(arestas, aresta);
            free(conflito);
       }
    };

    registers: NUMBER {
        int *conflito = malloc(sizeof *conflito);
        *conflito = $1;
        pilha_inserir(conflitos, conflito);
    } | NUMBER registers {
        int *conflito = malloc(sizeof *conflito);
        *conflito = $1;
        pilha_inserir(conflitos, conflito);
    };

%%

void yyerror (const char *msg) {
    grafo_destruir(grafo);
    pilha_destruir(conflitos);
    printf("ERROR: %s\n", msg);
    exit(1);
}

