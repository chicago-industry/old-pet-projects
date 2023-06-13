#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>

int f1(void)
{
	return (1);
}

int main(void) {
	
	int i = 0;
	if ((i = printf("отработала оригинальная\n")) > 1)
		printf("отработала оригинальная\n");
	else
		printf("перехватили\n");
	// printf("main: f1() -> "); f1(NULL); printf("\n");
	return 0;
}
