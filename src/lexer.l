%{
     #include "parser.tab.h"
%}

%option noyywrap
%option nounput noinput
%option outfile="lexer.y.c"

%%

"Grafo" {
    return GRAFO;
}

"K" {
     return K;
}

":" {
     return VIRGULA;
}

"=" {
     return IGUAL;
}

"-->" {
     return SETA;
}

(" "|[\t]) {}

[\n] {
     return NOVA_LINHA;
}

[0-9]+ {
     yylval.numero = atoi(yytext);
     return NUMERO;
}

. {
     printf("ERRO: lexer encontrou um caracter inválido: %s\n", yytext);
     exit(1);
}


%%
