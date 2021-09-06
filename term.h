#ifndef TERM_H
#define TERM_H

typedef struct term term_t;

typedef enum term_type {
    NODE_APPLICATION,
    NODE_ABSTRACTION,
    NODE_VARIABLE
} term_type_t;

typedef struct term_application {
    term_t *lterm;
    term_t *rterm;
} term_application_t;

typedef struct term_abstraction {
    char *arg;
    term_t *func;
} term_abstraction_t;

typedef struct term {
    term_type_t type;

    union {
        char *variable;
        term_application_t app;
        term_abstraction_t abs;
    };
} term_t;

term_t *
term_abstraction(char *arg, term_t *func);

term_t *
term_application(term_t *lterm, term_t *rterm);

term_t *
term_variable(char *variable);

term_t *
term_clone(const term_t *term);

term_t *
term_eval(const term_t *term);

void
term_free(term_t *term);

void
term_print(const term_t *term);

#endif