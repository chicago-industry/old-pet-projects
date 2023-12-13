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

	print("\nЗаписываем по адресу: ");
	print_address_hex(1, p[2]);
	print(" больше, чем выделели,\nпытаемся аллоцировать новый блок после этого\n");
	memset(p[2], 'a', size * 2);
	p[1] = malloc(size);

	show_alloc_mem();
	exit(0);
}
