%{
#include <stdio.h>
#include "lambda.h"

extern void yyerror(char const *s);
extern int yylex(void);
extern FILE* yyin;
%}

%union {
    char *variable;
    struct term *t;
}

%right LAMBDA
%left SPACE

%token<variable> VARIABLE
%token LAMBDA OP CP EOL EQ SPACE
%token EVAL

%type<t> term

%start start

%%

start: /* nothing */
    | start term EOL { print($2); prompt(); }
    | start VARIABLE EQ term EOL { assign($2, $4); prompt(); }
    ;

term: VARIABLE LAMBDA term { $$ = term_abstraction($1, $3); }
    | OP term CP { $$ = $2; }
    | term SPACE term { $$ = term_application($1, $3); }
    | VARIABLE { $$ = term_variable($1); }
    ;
%%

static void logo_print()
{
    printf("\n");
    printf("            ###########         \n");
    printf("        ##################      \n");
    printf("      #######        #######    \n");
    printf("     #####   #####      #####   \n");
    printf("    ####     ######       ####  \n");
    printf("   ####        ####        #### \n");
    printf("   ###        ######        ### \n");
    printf("   ###       ########       ### \n");
    printf("   ###      ####  ####      ### \n");
    printf("   ####    ####   #######  #### \n");
    printf("    ####  ####     ###### ####  \n");
    printf("     #####              #####   \n");
    printf("      ########      ########    \n");
    printf("        ##################      \n");
    printf("            ##########          \n");
}

int main(int argc, char **argv)
{
	yyin = stdin;
    symtab_init();

    logo_print();
    printf("\n> ");

	do {
		yyparse();
        fflush(stdin);
	} while(!feof(yyin));

    symtab_free();
	return 0;
}

void yyerror(char const *s)
{
    fprintf(stderr, "error: %s\n", s);
}