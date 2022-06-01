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
        grafo_destruir(grafo);
        pilha_destruir(conflitos);
        grafo = NULL;
    };

    header: name NEW_LINE max_registers NEW_LINE {
        grafo = grafo_criar($1, $3);
    };

    name: GRAPH NUMBER COLON {
        LOG_INFO("Grafo %d:\n", $2);
        $$ = $2;
    };

    max_registers: K ASSIGN NUMBER {
        LOG_INFO("K = %d\n", $3);
        $$ = $3;
    };

    body: conflict
        | conflict NEW_LINE body
    ;

    conflict: NUMBER ARROW registers {
        grafo_inserir_vertice(grafo, $1);
        int *conflito = NULL;
        while ((conflito = pilha_remover(conflitos)) != NULL) {
           grafo_inserir_aresta(grafo, $1, *conflito);
           free(conflito);
       }
    };

    registers: NUMBER {
        LOG_INFO("Register: %d\n", $1);
        int *conflito = malloc(sizeof *conflito);
        *conflito = $1;
        pilha_inserir(conflitos, conflito);
    }
             | NUMBER registers {
        LOG_INFO("Register: %d\n", $1);
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

