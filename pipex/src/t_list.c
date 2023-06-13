/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_list.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:57:49 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/08 18:57:50 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_list.h"

void	t_list_init_settings(t_list_set *set, void *next_addr, size_t size)
{
	set->next = (ssize_t)next_addr;
	set->size = size;
}

void	*t_list_add_front(void **head, t_list_set *set)
{
	char	*p;

	p = (char *)malloc(set->size);
	if (!p)
		return (NULL);
	else
	{
		ft_memset(p, 0, set->size);
		ft_memcpy(p + set->next, head, sizeof(void *));
		*head = p;
		return (*head);
	}
}

void	*t_list_add_back(void **head, void **last, t_list_set *set)
{
	char	*p;

	p = (char *)malloc(set->size);
	if (!p)
		return (NULL);
	else
	{
		ft_memset(p, 0, set->size);
		if (!(*head))
		{
			ft_memcpy(p + set->next, head, sizeof(void *));
			*head = p;
		}
		else
		{
			ft_memcpy(*last + set->next, &p, sizeof(void *));
		}
		*last = p;
		return (*last);
	}
}
