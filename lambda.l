%{
#include <stdio.h>
#include "lambda.tab.h"
#include "lambda.h"
#include "util.h"
%}

%%
" => " { return LAMBDA; }
" " { return SPACE; }
"(" { return OP; }
")" { return CP; }
\n { return EOL; }
" = " { return EQ; }

[A-Za-z0-9_+-]+ { yylval.variable = string_clone(yytext); return VARIABLE; }
. { printf("unknown character: %c", *yytext); prompt(); }
%%