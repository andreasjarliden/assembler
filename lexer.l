%{
#include "Argument.h"
#include "parser.tab.h"
#include "symbolTable.h"
#include <stdio.h>
%}

option nounput yylineno

IDENTIFIER [a-z][a-z0-9]*
DECNUMBER [0-9]+
HEXNUMBER [0-9a-fA-F]+h

/* rules */
%%

{IDENTIFIER} {
	printf("identifier -%s-\n", yytext);
	yylval.symbol = addSymbol(yytext);
	return IDENTIFIER;
}

{DECNUMBER} {
	printf("decnumber %s\n", yytext);
	yylval.numberValue = atoi(yytext);
	return DECNUMBER;
}

{HEXNUMBER} {
	printf("hexnumber %s\n", yytext);
	yylval.numberValue = strtol(yytext, NULL, 16);
	return HEXNUMBER;
}

{IDENTIFIER}: {
	printf("label %s-\n", yytext);
}

;.*$ {
	printf("comment %s\n", yytext);
}

[ \t] /* ignore whitespace */

^[ \t]*\n { printf("ignoring empty line\n"); yylineno++; }

, { return ','; }

\n {
	printf("newline\n");
	++yylineno;
	return NEWLINE;
}

%%
/* c code */

