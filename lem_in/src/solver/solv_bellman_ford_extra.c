/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_bellman_ford_extra.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 21:53:51 by pkingsbl          #+#    #+#             */
/*   Updated: 2020/07/27 23:57:16 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** count of active vertices of graph
*/

void			bellman_ford_num_vertices(t_graph *graph, t_bf *bf, \
											t_vertex *vertex)
{
	t_edge		*edge;
	t_vertex	*next_vertex;

	if (vertex->visited == 1)
		return ;
	vertex->visited = 1;
	bf->num_vertices++;
	edge = vertex->edge_forwd;
	while (edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, edge->key);
		if (next_vertex->visited == 0)
			bellman_ford_num_vertices(graph, bf, next_vertex);
		edge = edge->next;
	}
}
