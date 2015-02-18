%{
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "Argument.h"
int yylex(void);
void yyerror(char *s);
extern int yylineno;
%}

%union {
	const char* symbol;
	int numberValue;
	Argument argumentValue;
};

%token <number> DECNUMBER
%token <number> HEXNUMBER
%token <symbol> IDENTIFIER
%token <symbol> NEWLINE

%type <symbol> program
%type <argumentValue> argument
%type <numberValue> number

%%

program:
	program command { printf("p: Program %s\n", $1); }
	|
	;

command:
       	IDENTIFIER NEWLINE {
		printf("p: command -%s-\n", $1);
		(void)command0($1);
	}
       	| IDENTIFIER argument ',' argument NEWLINE {
		command2($1, &$2, &$4);
		printf("p: 2 args command %s argument types %d, %d\n", $1, $2.type, $4.type);
	}
       	| IDENTIFIER argument NEWLINE { printf("p: 1 arg command %s argument type %d\n", $1, $2.type); }
	;

argument:
	IDENTIFIER {
		printf("argument with identifier %s\n", $1);
		$$.type = 1;
		$$.identifier = $1;
	}
	| number {
		printf("argument with number %d\n", $1);
		$$.type = 2;
		$$.value = $1
	}
	;

number:
	DECNUMBER
	| HEXNUMBER
	;

%%

void yyerror(char *s) {
	fprintf(stderr, "%d:%s", yylineno, s);
}

