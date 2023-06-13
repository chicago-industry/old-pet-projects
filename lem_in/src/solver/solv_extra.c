/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_extra.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 21:59:22 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/27 20:56:45 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			clear_visited_room(t_graph *graph, t_vertex *vertex, \
									t_vertex *dest, int next_prev)
{
	t_edge		*edge;
	t_vertex	*next_vertex;

	if (vertex->visited == 0 && vertex != dest)
		return ;
	vertex->visited = 0;
	edge = next_prev ? vertex->edge_forwd : vertex->edge_back;
	while (edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, edge->key);
		clear_visited_room(graph, next_vertex, dest, next_prev);
		edge = edge->next;
	}
}

void			clear_visited(t_graph *graph)
{
	clear_visited_room(graph, graph->start, graph->start, EDGE_FORWD);
	clear_visited_room(graph, graph->start, graph->start, EDGE_BACK);
	clear_visited_room(graph, graph->end, graph->end, EDGE_FORWD);
	clear_visited_room(graph, graph->end, graph->end, EDGE_BACK);
}
