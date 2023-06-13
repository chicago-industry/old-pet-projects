#include "../include/malloc.h"

int main()
{
    void *p1, *p11, *p2, *p22;

    p22 = malloc(2000);
    p1 = malloc(5000);
    p11 = malloc(3000);
    p2 = malloc(7000);

    t_page *page;

    free(p2);
    show_alloc_mem();
    page = main_page_get()->large_last;
    for (; page; page = page->prev)
    {
        print_num(1, page->size);
        printl(1, "\n");
    }
    free(p11);
    // free(p2);
    // show_alloc_mem();
    // p = malloc(1025);
    // show_alloc_mem();
    // p = malloc(1025);
    // show_alloc_mem();
    // p = malloc(1025);
    // show_alloc_mem();
    // p = malloc(1025);
    // show_alloc_mem();
    return (0);
}