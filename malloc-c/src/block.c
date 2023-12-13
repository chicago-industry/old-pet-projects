#include "../include/malloc.h"

t_block	*block_place(void *area, const size_t area_size)
{
	t_block	*block = (t_block *)area;

	*block = (t_block){.magic_num = MAGIC_N, .size = area_size, .avail = AVAILABLE};
	return (block);
}

static void	block_check_rest_size(t_block *block, size_t area_size, size_t rest_size, t_page_type page_type)
{
	t_block	*block_possible;
	size_t	type_size;

	switch (page_type)
	{
	case P_LARGE:
		type_size = STRUCT_BLOCK_SIZE + BLOCK_SMALL_LIMIT;
		break;
	case P_SMALL:
		type_size = STRUCT_BLOCK_SIZE + BLOCK_TINY_LIMIT;
		break;
	case P_TINY:
		type_size = STRUCT_BLOCK_SIZE;
		break;
	}
	if (rest_size > type_size)
	{
		block_possible = block_place(BLOCK_JUMP(block, area_size), rest_size - STRUCT_BLOCK_SIZE);
		block_possible->prev = block;
		block_possible->next = block->next;
		block->next = block_possible;
	}
	else
		block->size = block->size + rest_size;
}

// block_reserve:
// if a avaiable matching block >= of the right size is found, we get here
// check if a new potential avaiable block can be placed behind the block
// (there may be the end of the page or another block)

void	block_reserve(t_block *block, size_t area_size, t_page_type page_type)
{
	size_t	rest_size;

	rest_size = BLOCK_JUMP(block, block->size) - BLOCK_JUMP(block, area_size);
	block->avail = UNAVAILABLE;
	block->size = area_size;
	block_check_rest_size(block, area_size, rest_size, page_type);
}

// block_get_available:
// look for a free block on pages, check for data overwriting (magic_num)
// if the pages are over, then create

t_block	*block_get_available(t_page *page, const size_t area_size)
{
	t_block	*block;

	if (!page && !(page = page_create(area_size)))
		return (NULL);
	for (block = page->block_head; block; block = block->next)
	{
		if (block->magic_num != MAGIC_N)
		{
			print_error_malloc(block, E_DATA);
			return (NULL);
		}
		else if (block->avail && block->size >= area_size)
		{
			block_reserve(block, area_size, page->type);
			page->block_unvail_qt++;
			return (block);
		}
	}
	return (block_get_available(page->next, area_size));
}
