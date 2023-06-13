/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_deadlock.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkingsbl <pkingsbl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 21:59:30 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 16:24:13 by pkingsbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** delete all deaadlock rooms which lead by edge_next
** do while at least one deadlock room found
*/

int				find_deadlock_room(t_graph *graph, t_vertex *vertex, \
									t_vertex *dest, t_vertex *prev_vertex)
{
	t_edge		*edge;
	t_vertex	*next_vertex;

	if (vertex->visited == 1)
		return (0);
	vertex->visited = 1;
	edge = vertex->edge_forwd;
	if (!edge->next && vertex != dest && vertex != graph->start)
	{
		del_edge_extra(&vertex->edge_forwd, vertex->edge_forwd->key);
		del_edge_extra(&prev_vertex->edge_forwd, vertex->key);
		return (1);
	}
	while (edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, edge->key);
		if (find_deadlock_room(graph, next_vertex, dest, vertex))
			return (1);
		edge = edge->next;
	}
	return (0);
}
