#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdbool.h>

char *
string_clone(const char *s);

bool
string_equal(const char *s1, const char *s2);

char *
string_append(const char *s, const char c);

void *
safe_malloc(size_t size);

void *
safe_calloc(size_t size);

#endif