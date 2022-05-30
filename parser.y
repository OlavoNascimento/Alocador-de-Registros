%{
    #include <iostream>
    #include "parser.tab.h"

    int yylex();
    void yyerror(const char *);

    using namespace std;
%}

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
%token RA_EOF

%start start

%%
    start: graph start {}
         | RA_EOF { exit(0); }
    ;

    graph: header body;

    header: name NEW_LINE max_registers NEW_LINE;

    name: GRAPH NUMBER COLON {
        cout << "Grafo " << $2 << ":" << endl;
    };

    max_registers: K ASSIGN NUMBER {
        cout << "K = " << $3 << endl;
    };

    body: conflict
        | conflict NEW_LINE body
    ;

    conflict: NUMBER ARROW registers {
        cout << "Edge: " << $1 << " ->> " << endl;
    };

    registers: NUMBER { cout << "Register: " << $1 << endl; }
            |  NUMBER registers { cout << "Register: " << $1 << endl; }

    ;

%%

// Report an error to the user.
void yyerror (const char *msg) {
    cerr << "ERROR: " << msg << endl;
}

