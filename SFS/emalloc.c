#include <stdlib.h>
#include <stdio.h>
#include "emalloc.h"

/*
 * This is Mike Zastre's code from SENG 265
 */

void *emalloc(size_t n) {
    void *p; 

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n); 
        exit(1);
    }   

    return p;
}
