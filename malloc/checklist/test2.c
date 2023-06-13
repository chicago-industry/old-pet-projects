#include <stdlib.h>

int main(void)
{
    int   i;
    char  *addr;

    i = 0;
    // void *p1 = malloc(1024);
    while (i < 1024) 
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        free(addr); 
        i++; 
    }
    // free(p1);
    return (0);
}