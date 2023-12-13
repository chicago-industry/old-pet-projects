/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_room_way.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkingsbl <pkingsbl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 19:21:14 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/10 17:14:49 by pkingsbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			free_room_way(t_room_way **head)
{
	t_room_way	*curr_node;

	if (!head)
		return ;
	while (*head)
	{
		curr_node = *head;
		ft_strdel(&curr_node->key);
		*head = (*head)->next;
		free(curr_node);
	}
}

void			init_room_way(t_room_way *room)
{
	room->key = NULL;
	room->ant = 0;
	room->x = 0;
	room->y = 0;
	room->next = NULL;
	room->last = NULL;
}

int				add_room_way(t_way *way, t_vertex *vertex)
{
	t_room_way	*new_room;
	t_room_way	*last_room;

	if (!way)
		return (0);
	if (!(new_room = node_malloc(sizeof(*new_room))))
		return (0);
	init_room_way(new_room);
	new_room->key = ft_strdup(vertex->key);
	new_room->x = vertex->x;
	new_room->y = vertex->y;
	if (!way->start_way)
	{
		new_room->last = new_room;
		way->start_way = new_room;
	}
	else
	{
		last_room = way->start_way->last;
		last_room->next = new_room;
		way->start_way->last = new_room;
	}
	return (1);
}
