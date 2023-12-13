/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_way_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 19:21:39 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/27 23:50:02 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int				is_start_vertex(t_graph *graph, t_way **way, t_vertex *vertex)
{
	if (vertex == graph->start)
	{
		if (!(add_way(way)))
			error(E_ADD_WAY, ERR_WAY);
		if (!(add_room_way((*way)->last, vertex)))
			error(E_ADD_ROOM_WAY, ERR_WAY);
		(*way)->last->count++;
		return (1);
	}
	return (0);
}

/*
** extra function of create_way function
** follow all edges from end_room which lead to the rooms with smallest bfs
** (from smallest to largest)
*/

t_vertex		*get_prefer_vertex(t_graph *graph, t_vertex *prev_vertex)
{
	t_edge		*end_edge;
	t_vertex	*prefer_vertex;
	t_vertex	*temp_vertex;
	int			bfs_min;

	end_edge = graph->end->edge_forwd;
	prefer_vertex = NULL;
	temp_vertex = NULL;
	bfs_min = 0x7FFFFFFF;
	while (end_edge)
	{
		temp_vertex = hashtab_lookup(graph->hashtab, end_edge->key);
		if (temp_vertex != prev_vertex && temp_vertex->visited == 0 \
			&& (temp_vertex->bfs < bfs_min))
		{
			bfs_min = temp_vertex->bfs;
			prefer_vertex = hashtab_lookup(graph->hashtab, end_edge->key);
		}
		end_edge = end_edge->next;
	}
	return (prefer_vertex);
}

int				create_way_extra(t_graph *graph, t_way **way, t_vertex *vertex)
{
	t_edge		*edge;
	t_vertex	*next_vertex;

	if (vertex->visited == 1 && vertex != graph->start)
		return (0);
	vertex->visited = 1;
	if (is_start_vertex(graph, way, vertex))
		return (1);
	edge = vertex->edge_forwd;
	while (edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, edge->key);
		if (edge->cost == -1)
		{
			if (create_way_extra(graph, way, next_vertex))
			{
				if (!(add_room_way((*way)->last, vertex)))
					error(E_ADD_WAY, ERR_WAY);
				(*way)->last->count++;
				return (1);
			}
		}
		edge = edge->next;
	}
	return (0);
}

/*
** recursion
** start from the end and find edge to room with smallest bfs
** trying to get to the start
** in case of success, form the found way to t_way *way
** if no unvisited rooms, break
** return number of ways
*/

int				create_way(t_graph *graph, t_way **way)
{
	t_vertex	*prefer_vertex;
	int			count_ways;

	count_ways = 0;
	while (1)
	{
		prefer_vertex = get_prefer_vertex(graph, graph->end);
		if (!prefer_vertex)
			break ;
		if (create_way_extra(graph, way, prefer_vertex))
		{
			if (!(add_room_way((*way)->last, graph->end)))
				error(E_ADD_WAY, ERR_WAY);
			(*way)->last->count++;
			count_ways++;
		}
	}
	return (count_ways);
}
