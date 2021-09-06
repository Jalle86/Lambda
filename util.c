#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
string_clone(const char *s)
{
    char *clone = safe_calloc(strlen(s) + 1);

    strcpy(clone, s);

    return clone;
}

bool
string_equal(const char *s1, const char *s2)
{
    return strcmp(s1, s2) == 0;
}

char *
string_append(const char *s, const char c)
{
    size_t len = strlen(s) + 2;
    char *app = safe_calloc(len);

    strcpy(app, s);
    app[len - 2] = c;

    return app;
}

void *
safe_malloc(size_t size)
{
    void *data = malloc(size);
    if (data == NULL) {
        printf("Out of memory");
        exit(0);
    }
    else {
        return data;
    }
}

void *
safe_calloc(size_t size)
{
    void *data = calloc(1, size);
    if (data == NULL) {
        printf("Out of memory");
        exit(0);
    }
    else {
        return data;
    }
}