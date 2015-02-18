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
	struct Argument argumentValue;
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
	program line
	|
	;

line:
    NEWLINE
    | statement NEWLINE;

statement:
	  command
	| label
	;

command:
       	IDENTIFIER {
		printf("p: command -%s-\n", $1);
		(void)command0($1);
	}
       	| IDENTIFIER argument ',' argument {
		command2($1, &$2, &$4);
		printf("p: 2 args command %s argument types %d, %d\n", $1, $2.type, $4.type);
	}
       	| IDENTIFIER argument {
		command1($1, &$2);
		printf("p: 1 arg command %s argument type %d\n", $1, $2.type);
	}
	;

label:
	IDENTIFIER ':' {
		printf("label %s\n", $1);
		label($1);
	}
	;

argument:
	IDENTIFIER {
		printf("argument with identifier %s\n", $1);
		$$.type = IDENTIFIER_ARGUMENT;
		$$.identifier = $1;
	}
	| number {
		printf("argument with number %d\n", $1);
		$$.type = VALUE_ARGUMENT;
		$$.value = $1
	}
	| '(' number ')' {
		printf("argument with address %d\n", $2);
		$$.type = ADDRESS_ARGUMENT;
		$$.value = $2
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

