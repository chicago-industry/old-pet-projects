#include "malloc.h"

int main()
{
    void *p;

    p = malloc(5000);
    show_alloc_mem();
    free(p);
    show_alloc_mem();
    p = malloc(1025);
    show_alloc_mem();
    p = malloc(1025);
    show_alloc_mem();
    p = malloc(1025);
    show_alloc_mem();
    p = malloc(1025);
    show_alloc_mem();
    return (0);
}