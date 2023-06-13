#define _GNU_SOURCE

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>

#if !(__x86_64__)
	#error "This only works on x86_64"
#endif

extern void f1(void);
// extern void f2(void);

__asm__("pop %rax");
int trampoline(void)
{
	// printf("intercepted by trampoline() -> ");
	// f2();
	return (0);
}

/* TODO: make less ugly! there's got to be a nicer way to do this! */
#pragma pack(push, 1)
struct
{
	char push_rax;
	char mov_rax[2];
	char addr[8];
	char jmp_rax[2];
} jump_asm = {
	.push_rax = 0x50,
	.mov_rax = {0x48, 0xb8},
	.jmp_rax = {0xff, 0xe0}};
#pragma pack(pop)

void __attribute__((constructor)) init(void)
{
	int pagesize = sysconf(_SC_PAGE_SIZE);
	char *page = (char *)printf;

	page = (char *)((size_t)page & ~(pagesize - 1));
	int rc = mprotect(page, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC);
	if (rc)
	{
		fprintf(stderr, "mprotect() failed.\n");
		return;
	}
	void *addr = &trampoline - 1;
	memcpy(jump_asm.addr, &addr, sizeof(void *));
	memcpy((void *)printf, &jump_asm, sizeof jump_asm);
}
