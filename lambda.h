#ifndef LAMBDA_H
#define LAMBDA_H

#include <stdlib.h>
#include "term.h"

void
symtab_init(void);

void
symtab_free(void);

void
symtab_set(char *variable, term_t *term);

void
assign(char *variable, term_t *term);

void
print(const term_t *term);

void
prompt(void);

#endif