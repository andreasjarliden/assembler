%{
#include "Argument.h"
#include "parser.tab.h"
#include "stringTable.h"
#include <stdio.h>
%}

option nounput yylineno

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_-]*
DECNUMBER [0-9]+
HEXNUMBER [0-9a-fA-F]+h

/* rules */
%%

{IDENTIFIER} {
	yylval.symbol = addSymbol(yytext);
	return IDENTIFIER;
}

{DECNUMBER} {
	yylval.numberValue = atoi(yytext);
	return DECNUMBER;
}

{HEXNUMBER} {
	yylval.numberValue = strtol(yytext, NULL, 16);
	return HEXNUMBER;
}

[ \t] /* ignore whitespace */

^[ \t]*\n { yylineno++; }

[,():.] { return yytext[0]; }

(;.*)?\n {
	++yylineno;
	return NEWLINE;
}

%%
/* c code */

