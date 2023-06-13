#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

#define QT 25
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
		memset(p[i], 'a', size);
	}
	show_alloc_mem();

	print("\nОсвобождаем каждый четный\n\n");
	for (int i = 0; i < QT; i++)
	{
		if (i % 2)
			free(p[i]);
	}
	show_alloc_mem();

	print("\nОсвобождаем указатель по адресу: ");
	print_address_hex(1, p[2]);
	print(" (1024 * 3 + (2 * 40 (два пустых мета блока)) == 3152 \n\n");
	free(p[2]);
	show_alloc_mem();

	print("\nОсвобождаем все, кроме первого\n\n");
	for (int i = 1; i < QT; i++)
	{
		if (!(i % 2) && i != 2) 
			free(p[i]);
	}
	show_alloc_mem();

	print("\nПробуем освободить неаллоцированный указатель\n\n");
	free(p[1]);

	exit(0);
}
