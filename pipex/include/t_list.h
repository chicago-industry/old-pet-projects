/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_list.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:19:04 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:21:30 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_LIST_H
# define T_LIST_H

# include "libfuck.h"

# include <sys/types.h>

// It should have been a separate lib with many instruments
// to create/manage linked lists, binary trees, etc
// but i'm too lazy...
// Maybe, one day!

typedef struct s_list_set
{
	ssize_t		next;
	ssize_t		prev;
	size_t		size;
	void		*head;
	void		*last;
}	t_list_set;

void	t_list_init_settings(t_list_set *set, void *next_addr, size_t size);
void	*t_list_add_front(void **head, t_list_set *set);
void	*t_list_add_back(void **head, void **last, t_list_set *set);

#endif
