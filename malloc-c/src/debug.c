#include "../include/malloc.h"

#define DB_TINY		"TINY:\n"
#define DB_SMALL	"SMALL:\n"
#define DB_LARGE	"LARGE:\n"

#define DB_PAGE		"page: "
#define DB_BYTE		" bytes\n"

#define DB_INDENT	"      "
#define DB_AV		"AV   "
#define DB_UNAV		"0    "

static void	dbg_count_blocks_in_page(const t_page *page)
{
	t_block	*block;
	size_t	i = 1;

	for (block = page->block_head; block && block->magic_num == MAGIC_N ; block = block->next, i++)
	{
		printl(1, DB_INDENT);
		print_address_hex(1, BLOCK_JUMP(block, 0));
		printl(1, " - ");
		print_address_hex(1, BLOCK_JUMP(block, block->size));
		printl(1, " ");
		if (block->avail)
			printl(1, DB_AV);
		else
			printl(1, DB_UNAV);
		print_num(1, block->size);
		printl(1, DB_BYTE);
	}
}

static void	show_alloc_mem_extra(const t_page *page)
{
	size_t	i = 0;

	switch (page->type)
	{
	case P_TINY :
		printl(1, DB_TINY);
		break;
	case P_SMALL :
		printl(1, DB_SMALL);
		break;
	case P_LARGE :
		printl(1, DB_LARGE);
		break;
	}
	for (; page; i++, page = page->next)
	{
		printl(1, DB_PAGE);
		print_address_hex(1, (t_v)page);
		printl(1, " ");
		print_num(1, page->size);
		printl(1, DB_BYTE);
		dbg_count_blocks_in_page(page);
	}
}

void	show_alloc_mem(void)
{
	t_main_page	*main_page;

	main_page = main_page_get();
	if (main_page->tiny_head)
		show_alloc_mem_extra(main_page->tiny_head);
	if (main_page->small_head)
		show_alloc_mem_extra(main_page->small_head);
	if (main_page->large_head)
		show_alloc_mem_extra(main_page->large_head);
}
