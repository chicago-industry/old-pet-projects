#include "../include/malloc.h"

static void	dealloc_memory_extra(t_page **head, t_page **last, t_page *page)
{
	t_page	*tmp;

	if (*last == page)
		*last = page->prev;
	if (*head == page)
	{
		page->prev = NULL;
		*head = page->next;
		munmap((t_v)page, page->size);
		return ;
	}
	for (tmp = *head; tmp->next; tmp = tmp->next)
	{
		if (tmp->next == page)
		{
			tmp->next = page->next;
			if (page->next)
				page->next->prev = tmp;
			munmap((t_v)page, page->size);
			return ;
		}
	}
}

static void	dealloc_memory(t_page *page)
{
	t_main_page	*main_page;

	main_page = main_page_get();
	switch (page->type)
	{
	case P_TINY :
		dealloc_memory_extra(&main_page->tiny_head, &main_page->tiny_last, page);
		break;
	case P_SMALL :
		dealloc_memory_extra(&main_page->small_head, &main_page->small_last, page);
		break;
	case P_LARGE :
		dealloc_memory_extra(&main_page->large_head, &main_page->large_last, page);
		break;
	}
}

static void	free_defragmentation(t_block *block)
{
	t_block	*temp;

	temp = block;
	if (temp->prev && temp->prev->avail)
	{
		temp->magic_num = 0;
		temp = temp->prev;
	}
	if (block->next && block->next->avail)
	{
		block->next->magic_num = 0;
		block = block->next;
	}
	if (block->next)
		block->next->prev = temp;
	temp->next = block->next;
	temp->size = BLOCK_JUMP(block, block->size) - BLOCK_JUMP(temp, 0);
}

t_block	*block_validation(t_page *page, void *ptr)
{
	t_block		*block;

	for (block = page->block_head; block; block = block->next)
	{
		if (block->magic_num != MAGIC_N)
		{
			print_error_malloc(block, E_DATA);
			return (NULL);
		}
		if (block == BLOCK_META(ptr))
			break ;
	}
	if (!block || block->avail)
	{
		print_error_malloc(ptr, E_PTR);
		return (NULL);
	}
	return (block);
}

static t_page	*page_find(t_page *page, void *ptr)
{
	for (; page; page = page->next)
	{
		if (ptr > (t_v)page && ptr < (t_v)PAGE_JUMP(page, page->size - STRUCT_PAGE_SIZE))
			break ;
	}
	return (page);
}

t_page	*page_validation(void *ptr)
{
	t_main_page	*main_page;
	t_page		*page;

	main_page = main_page_get();
	if (!(page = page_find(main_page->tiny_head, ptr)))
	{
		if (!(page = page_find(main_page->small_head, ptr)))
		{
			if (!(page = page_find(main_page->large_head, ptr)))
				print_error_malloc(ptr, E_PTR);
		}
	}
	return (page);
}

// free:
// check if ptr exists on pages
// check if ptr is a part of real block with meta data
// sets ptr to available
// 'defragmentation':
// if no more unavailable block on the page call munmap()
// but if there is only one page left, do not call munmap()
// otherwise, we check if we can combine adjacent available blocks
// into one free area, which will be a new available block 

void	free(void *ptr)
{
	t_page	*page;
	t_block	*block;

	if (ptr)
	{
		if ((page = page_validation(ptr)))
		{
			if ((block = block_validation(page, ptr)))
			{
				block->avail = AVAILABLE;
				page->block_unvail_qt--;
				if (page->block_unvail_qt < 1 && page->prev)
					dealloc_memory(page);
				else
					free_defragmentation(block);
			}
		}
	}
}
