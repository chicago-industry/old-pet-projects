/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_way.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkingsbl <pkingsbl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 19:21:09 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/10 17:15:00 by pkingsbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			free_way(t_way **way)
{
	t_way	*curr_node;

	if (!way)
		return ;
	while (*way)
	{
		curr_node = *way;
		free_room_way(&(*way)->start_way);
		*way = (*way)->next;
		free(curr_node);
	}
	*way = NULL;
}

void			init_way(t_way *way)
{
	way->count = 0;
	way->ant_on_way = 0;
	way->start_way = NULL;
	way->next = NULL;
	way->last = NULL;
}

int				add_way(t_way **way)
{
	t_way		*new_way;
	t_way		*last_way;

	if (!way)
		return (0);
	if (!(new_way = node_malloc(sizeof(*new_way))))
		return (0);
	init_way(new_way);
	if (!*way)
	{
		new_way->last = new_way;
		(*way) = new_way;
	}
	else
	{
		last_way = (*way)->last;
		last_way->next = new_way;
		(*way)->last = new_way;
	}
	return (1);
}
