#include <string.h>
#include "utils.h"

char *appendString(char *a, const char *b)
{
    if (!b)
        return a; // nothing to append

    if (!a)
    { // handle NULL base string
        a = malloc(strlen(b) + 1);
        if (!a)
            return NULL;
        strcpy(a, b);
        return a;
    }

    size_t len_a = strlen(a);
    size_t len_b = strlen(b);

    char *tmp = realloc(a, len_a + len_b + 1);
    if (!tmp)
    {
        free(a);
        return NULL;
    }

    memcpy(tmp + len_a, b, len_b + 1); // safe append including '\0'
    return tmp;
}

unsigned char randColor()
{
    return (unsigned char)(rand() % 256);
}