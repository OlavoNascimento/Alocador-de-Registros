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

