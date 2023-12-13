#include "../include/malloc.h"

static void	main_page_update_extra(t_page **head, t_page **last, t_page *new)
{
	if (!*head)
		*head = new;
	else
	{
		(*last)->next = new;
		new->prev = *last;
	}
	*last = new;
}

// if the last page is larger than the first (size),
// then we put the last one in front
// to do not call munmap even if it will be available

static void	main_page_swap_large_extra(t_page **head, t_page **last)
{
	t_page	*temp;

	if (*head != *last)
	{
		if ((*head)->next == *last)
		{
			temp = *head;
			(*last)->next = *head;
			(*last)->prev = NULL;
			(*head)->prev = *last;
			(*head)->next = NULL;
		}
		else
		{
			temp = (*last)->prev;
			(*last)->prev->next = NULL;
			(*last)->next = *head;
			(*last)->prev = NULL;
			(*head)->prev = *last;
		}
		*head = *last;
		*last = temp;
	}
}

// main_page_update:
// a.k.a push back

void	main_page_update(t_page *page)
{
	t_main_page	*m_page;

	m_page = main_page_get();
	switch (page->type)
	{
	case P_TINY :
		main_page_update_extra(&m_page->tiny_head, &m_page->tiny_last, page);
		break;
	case P_SMALL :
		main_page_update_extra(&m_page->small_head, &m_page->small_last, page);
		break;
	case P_LARGE :
		main_page_update_extra(&m_page->large_head, &m_page->large_last, page);
		if (m_page->large_last->size > m_page->large_head->size)
			main_page_swap_large_extra(&m_page->large_head, &m_page->large_last);
		break;
	}
}

// main_page_get:
// returns the address of a static structure that stores information about pages
// of different types

t_main_page	*main_page_get(void)
{
	static t_main_page	main_page;

	return (&main_page);
}
