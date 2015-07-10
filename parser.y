%{
#include <stdio.h>
#include <string.h>
#include <command.h>
#include <Argument.h>
int yylex(void);
void yyerror(char *s);
extern int yylineno;
%}

%union {
	const char* symbol;
	int numberValue;
	struct RawArgument argumentValue;
};

%token <numberValue> DECNUMBER
%token <numberValue> HEXNUMBER
%token <symbol> IDENTIFIER
%token <symbol> NEWLINE
%token <symbol> STRING

%type <symbol> program
%type <argumentValue> argument
%type <numberValue> number

%%

program:
	program line
	| { /* empty rules have no default action, so this avoids a warning*/ }
	;

line:
    NEWLINE
    | statement NEWLINE;

statement:
	  label genericCommand
	| label
	| genericCommand
	;

genericCommand:
          command
	| metaCommand
	;

command:
       	IDENTIFIER {
		(void)command0($1);
	}
       	| IDENTIFIER argument ',' argument {
		command2($1, &$2, &$4);
	}
       	| IDENTIFIER argument {
		command1($1, &$2);
	}
	;

label:
	IDENTIFIER ':' {
		label($1);
	}
	;

metaCommand:
	'.' IDENTIFIER argument {
		metaCommand1($2, &$3);
	}
	|
	'.' IDENTIFIER IDENTIFIER argument {
		metaCommand2($2, $3, &$4);
	}
	;

argument:
	IDENTIFIER {
		$$.type = IDENTIFIER_ARGUMENT;
		$$.identifier = $1;
	}
	| number {
		$$.type = VALUE_ARGUMENT;
		$$.value = $1
	}
	| '(' number ')' {
		$$.type = ADDRESS_VALUE_ARGUMENT;
		$$.value = $2
	}
	| '(' IDENTIFIER ')' {
		$$.type = ADDRESS_IDENTIFIER_ARGUMENT;
		$$.identifier = $2
	}
	| STRING {
		$$.type = STRING_ARGUMENT;
		$$.identifier = $1;
	}
	;

number:
	DECNUMBER
	| HEXNUMBER
	;

%%

void yyerror(char *s) {
	extern const char* filename;
	extern int NUMBER_OF_ERRORS;
	fprintf(stderr, "%s:%d: error: %s\n", filename, yylineno, s);
	++NUMBER_OF_ERRORS;
}

