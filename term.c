#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "term.h"
#include "util.h"

static term_t *
replace(const term_t *term, const char *variable, const term_t *replaced);

term_t *
term_abstraction(char *variable, term_t *t)
{
    term_t *term = safe_calloc(sizeof(*term));

    term->type = NODE_ABSTRACTION;
    term->abs.func = t;
    term->abs.arg = variable;

    return term;
}

term_t *
term_application(term_t *lterm, term_t *rterm)
{
    term_t *term = safe_calloc(sizeof(*term));

    term->type = NODE_APPLICATION;
    term->app.lterm = lterm;
    term->app.rterm = rterm;

    return term;
}

term_t *
term_variable(char *variable)
{
    term_t *term = safe_calloc(sizeof(*term));

    term->type = NODE_VARIABLE;
    term->variable = variable;

    return term;
}

term_t *
term_clone(const term_t *term)
{
    switch(term->type) {
        case NODE_VARIABLE:
        return term_variable(string_clone(term->variable));
        case NODE_APPLICATION:
        return term_application(term_clone(term->app.lterm), term_clone(term->app.rterm));
        case NODE_ABSTRACTION:
        return term_abstraction(string_clone(term->abs.arg), term_clone(term->abs.func));
        default:
            printf("Unknown type: %d", term->type);
            exit(0);
    }
}

void
term_free(term_t *term) {
    switch(term->type) {
        case NODE_ABSTRACTION:
            term_free(term->abs.func);
            free(term->abs.arg);
            break;
        case NODE_APPLICATION:
            term_free(term->app.lterm);
            term_free(term->app.rterm);
            break;
        case NODE_VARIABLE:
            free(term->variable);
            break;
        default:
            printf("Unknown type: %d", term->type);
            exit(0);
    }

    free(term);
}

void
term_print(const term_t *term)
{
    switch(term->type) {
        case NODE_VARIABLE:
            printf("%s", term->variable);
            break;
        case NODE_ABSTRACTION:
            printf("%s => ", term->abs.arg);
            term_print(term->abs.func);
            break;
        case NODE_APPLICATION:
            if (term->app.lterm->type == NODE_ABSTRACTION) {
                printf("(");
                term_print(term->app.lterm);
                printf(")");
                
            }
            else {
                term_print(term->app.lterm);
            }

            printf(" ");

            if (term->app.rterm->type != NODE_VARIABLE) {
                printf("(");
                term_print(term->app.rterm);
                printf(")");
                
            }
            else {
                term_print(term->app.rterm);
            }

            break;
        default:
            printf("Unknown type: %d", term->type);
            exit(0);
    }
}

term_t *
term_eval(const term_t *term)
{
    switch(term->type) {
        case NODE_APPLICATION: {
            term_t *lterm = term->app.lterm;
            term_t *rterm = term->app.rterm;

            if (lterm->type == NODE_ABSTRACTION) {
                term_t *lterm_reduced = replace(lterm->abs.func, lterm->abs.arg, term_clone(rterm));
                return term_eval(lterm_reduced);
            }
            else {
                term_t *app = term_application(term_eval(lterm), term_eval(rterm));
                if (app->app.lterm->type == NODE_ABSTRACTION) {
                    term_t *eval = term_eval(app);
                    term_free(app);
                    return eval;
                }
                else {
                    return app;
                }
            }
        }

        case NODE_VARIABLE:
            return term_variable(string_clone(term->variable));

        case NODE_ABSTRACTION:
            return term_abstraction(string_clone(term->abs.arg), term_eval(term->abs.func));

        default:
            printf("Unknown type: %d", term->type);
            exit(0);

    }
    return NULL;
}

static term_t *
replace(const term_t *term, const char *variable, const term_t *replaced)
{
    switch(term->type) {
        case NODE_VARIABLE:
            return string_equal(term->variable, variable) ? term_clone(replaced): term_clone(term);

        case NODE_APPLICATION: {
            term_t *lterm = replace(term->app.lterm, variable, replaced);
            term_t *rterm = replace(term->app.rterm, variable, replaced);
            return term_application(lterm, rterm);
        }

        case NODE_ABSTRACTION:
            if (string_equal(term->abs.arg, variable)) {
                char *arg = string_clone(variable);
                char *append = string_append(arg, '\'');
                term_t *append_term = term_variable(string_clone(append));

                term_t *replaced = term_abstraction(string_clone(append), replace(term->abs.func, variable, append_term));

                free(arg);
                term_free(append_term);

                return replaced;

            }
            else {
                return term_abstraction(string_clone(term->abs.arg), replace(term->abs.func, variable, replaced));
            }

        default:
            printf("Unknown type: %d", term->type);
            exit(0);
    }
}