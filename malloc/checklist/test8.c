#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

#define QT 3
#define SIZE 1024

void *p[QT];
size_t size = SIZE;

void print(char *s)
{
    write(1, s, strlen(s));
}

int main()
{
	for (int i = 0; i < QT; i++)
	{
		if (!(p[i] = malloc(size)))
		{
			print("stopped: malloc returned NULL\n");
			show_alloc_mem();
			exit(1);
		}
		else
			memset(p[i], 'a', size);
	}
	show_alloc_mem();

	print("\nреаллоцируем данные по адресу: ");
	print_address_hex(1, p[1]);
	print(" на меньший размер (200 b)\n\n");
	p[1] = realloc(p[1], 200);
	show_alloc_mem();


	print("\nреаллоцируем данные по адресу: ");
	print_address_hex(1, p[0]);
	print(" на больший размер (2000 b)\n");
	p[0] = realloc(p[0], 2000);
	print("теперь данные находятся по адресу: ");
	print_address_hex(1, p[0]);
	print("\n\n");
	show_alloc_mem();

	exit(0);
}
