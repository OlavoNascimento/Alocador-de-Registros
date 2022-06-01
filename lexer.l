%{
     #include "parser.tab.h"
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
     yylval.number = atoi(yytext);
     return NUMBER;
}

. {
     printf("ERROR: lexer found invalid character: %s\n", yytext);
     exit(1);
}


%%
