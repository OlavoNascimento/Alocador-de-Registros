%{
     #include <stdio.h>
     #include <string.h>
     #include <iostream>

     #include "parser.tab.h"

     using namespace std;
%}

%option noyywrap
%option nounput noinput
%option outfile="lexer.y.c"

%%

"Grafo" {
    return GRAPH;
}

"K" {
     return K;
}

":" {
     return COLON;
}

"=" {
     return ASSIGN;
}

"-->" {
     return ARROW;
}

(" "|[\t]) {}

[\n] {
     return NEW_LINE;
}

[0-9]+ {
     char *end;
     yylval.number = strtoul(yytext, &end, 10);
     return NUMBER;
}

. {
     cerr << "ERROR: lexer found invalid character: " << yytext << endl;
     exit(1);
}

<<EOF>> {
     return RA_EOF;
}

%%
