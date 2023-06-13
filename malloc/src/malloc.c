#include "../include/malloc.h"

void	*alloc_memory(const size_t page_size)
{
	void	*data_memory;

	data_memory = mmap(NULL, \
						page_size, \
						PROT_READ | PROT_WRITE, \
						MAP_PRIVATE | MAP_ANON, \
						-1, \
						0);
	if (data_memory == MAP_FAILED)
		return (NULL);
	return (data_memory);
}

// calloc:
// overflow check
// call malloc
// fill by zero the memory area

void	*calloc(size_t nmemb, size_t memb_size)
{
	void	*area;
	size_t	size;

	area = NULL;
	if (nmemb && memb_size)
	{
		size = nmemb * memb_size;
		if (size / memb_size != nmemb)
			print_error_malloc(NULL, E_SIZE);
		else if ((area = malloc(size)))
			ft_memset(area, 0, size);
	}
	return (area);
}

// malloc:
// overflow check
// get the current or create a page of the required type
// looking for a available block

void	*malloc(size_t area_size)
{
	t_page *page;
	t_block	*block;

	if (area_size + STRUCT_PAGE_SIZE + STRUCT_BLOCK_SIZE < area_size)
		print_error_malloc(NULL, E_SIZE);
	else
	{
		if ((page = page_get_available(area_size)))
		{
			if ((block = block_get_available(page, area_size)))
				return ((t_v)BLOCK_JUMP(block, 0));
		}
	}
	return (NULL);
}

// realloc:
// if ptr doest'n exist, calls malloc
// if ptr exist and size is 0, calls free
// looking for allocated block, changes to area_size, do reallocate of data if needed

void	*realloc(void *ptr, size_t area_size)
{
	t_page	*page;
	t_block *block;
	void	*block_new;

	if (!ptr)
		return (malloc(area_size));
	else if (!area_size)
		free(ptr);
	if ((page = page_validation(ptr)))
	{
		if ((block = block_validation(page, ptr)))
		{
			if (area_size > block->size)
			{
				if ((block_new = malloc(area_size)))
				{
					ft_memcpy(block_new, BLOCK_JUMP(block, 0), block->size);
					free((BLOCK_JUMP(block, 0)));
					return (block_new);
				}
			}
			else
			{
				block_reserve(block, area_size, page->type);
				return (ptr);
			}
		}
	}
	return (NULL);
}
