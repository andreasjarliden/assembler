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
  char charValue;
	struct RawArgument argumentValue;
};

%token <numberValue> DECNUMBER
%token <numberValue> HEXNUMBER
%token <numberValue> CHARNUMBER
%token <charValue> PLUS_OR_MINUS
%token <symbol> IDENTIFIER
%token <symbol> NEWLINE
%token <symbol> STRING

%type <symbol> program
%type <argumentValue> argument
%type <numberValue> number
%type <symbol> identifier

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
     globalLabel
     | localLabel
     ;

globalLabel:
	IDENTIFIER ':' {
		label($1);
	}
	;

localLabel:
  '.' IDENTIFIER ':' {
    localLabel($2);
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
	identifier {
		$$.type = IDENTIFIER_ARGUMENT;
		$$.identifier = $1;
    $$.value = 0;
	}
	| number {
		$$.type = VALUE_ARGUMENT;
		$$.value = $1;
    $$.identifier = 0;
	}
	| '(' number ')' {
		$$.type = DEREFERENCED_VALUE_ARGUMENT;
		$$.value = $2;
    $$.identifier = 0;
	}
	| '(' identifier ')' {
		$$.type = DEREFERENCED_IDENTIFIER_ARGUMENT;
		$$.identifier = $2;
    $$.value = 0;
	}
	| '(' identifier PLUS_OR_MINUS number ')' {
		$$.type = DEREFERENCED_INDEXED_IDENTIFIER_ARGUMENT;
		$$.identifier = $2;
    $$.indexOperation = $3;
    $$.value = 0;
    $$.indexValue = $4;
    $$.indexIdentifier = 0;
	}
	| '(' identifier PLUS_OR_MINUS IDENTIFIER ')' {
		$$.type = DEREFERENCED_INDEXED_IDENTIFIER_ARGUMENT;
		$$.identifier = $2;
    $$.indexOperation = $3;
    $$.value = 0;
    $$.indexValue = 0;
    $$.indexIdentifier = $4;
	}
	| STRING {
		$$.type = STRING_ARGUMENT;
		$$.identifier = $1;
    $$.value = 0;
	}
	;

// Also support local labels .foo as identifiers
identifier:
    IDENTIFIER
    | '.' IDENTIFIER { $$ = resolveLocalLabel($2); }
    ;

number:
	DECNUMBER
	| HEXNUMBER
	| CHARNUMBER
	;

%%

void yyerror(char *s) {
	extern const char* filename;
	extern int NUMBER_OF_ERRORS;
	fprintf(stderr, "%s:%d: error: %s\n", filename, yylineno, s);
	++NUMBER_OF_ERRORS;
}

