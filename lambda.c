#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "lambda.h"
#include "util.h"

GHashTable *symtab = NULL;

static void
value_destroy(void *value)
{
    term_free((term_t *)value);
}

void
symtab_init(void)
{
    symtab = g_hash_table_new_full(g_str_hash, g_str_equal, free, value_destroy);
}

void
symtab_set(char *variable, term_t *term)
{
    g_hash_table_insert(symtab, variable, term);
}

void
symtab_free(void)
{
    g_hash_table_destroy(symtab);
}

term_t *
eval(const term_t *term)
{
    GHashTableIter iter;
    char *iter_variable;
    term_t *iter_term;

    term_t *globs = term_clone(term);

    g_hash_table_iter_init(&iter, symtab);
    while (g_hash_table_iter_next (&iter, (void *)&iter_variable, (void *)&iter_term)) {
        globs = term_application(term_abstraction(string_clone(iter_variable), globs), term_clone(iter_term));
    }

    term_t *e = term_eval(globs);

    term_free(globs);

    return e;
}

void print(const term_t *term)
{
    term_t *e = eval(term);
    term_print(e);
    term_free(e);
}

void
assign(char *variable, term_t *term)
{
    term_t *t = eval(term);
    term_print(t);
    symtab_set(variable, term);
}

void
prompt(void)
{
    printf("\n\n> ");
}