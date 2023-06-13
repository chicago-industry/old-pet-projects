/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_find_way.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 19:21:48 by yshawn            #+#    #+#             */
/*   Updated: 2020/08/12 20:11:49 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** return (1) if any edge_back from vertex has cost -1:
** vertex is busy (other way built with this vertex)
** otherwise return (0)
*/

int				is_busy_vertex(t_vertex *vertex, char *came)
{
	t_edge		*temp_edge;

	temp_edge = vertex->edge_forwd;
	while (temp_edge)
	{
		if (temp_edge->cost == -1 && ft_strcmp(temp_edge->key, came) != 0)
			return (1);
		temp_edge = temp_edge->next;
	}
	return (0);
}

/*
** return the edge that link with biggest unvisited vertex from edge_forwd
** otherwise return NULL
*/

t_edge			*get_prefer_edge(t_graph *graph, t_vertex *vertex, \
									t_vertex *prev_vertex)
{
	t_edge		*temp_edge;
	t_edge		*prefer_vertex;
	t_vertex	*next_vertex;
	int			bf_min;

	prefer_vertex = NULL;
	bf_min = 0x7FFFFFFF;
	temp_edge = vertex->edge_forwd;
	while (temp_edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, temp_edge->key);
		if (next_vertex != prev_vertex && next_vertex->visited == 0 \
			&& temp_edge->cost == 1 && (next_vertex->cost < bf_min))
		{
			bf_min = next_vertex->cost;
			prefer_vertex = temp_edge;
		}
		temp_edge = temp_edge->next;
	}
	return (prefer_vertex);
}

/*
** to find any intersections that interfere with other ways
** we should go by edge_forwd edge (second priority)
** at first find biggest bfs vertex from edge_forwd edges:
** vertex must be not visited
** edge must be not busy and have cost == 1
** if next_vertex that we want to visit already
** has any edge in edge_back that has cost == -1:
** the next_vertex is considered occupied
** then delete edges between next_vertex and curr_vertex
*/

int				find_way_forward(t_graph *graph, t_vertex *curr_vertex, \
									t_vertex *prev_vertex)
{
	t_edge		*prefer_edge;
	t_vertex	*next_vertex;

	while (1)
	{
		if (!(prefer_edge = get_prefer_edge(graph, curr_vertex, prev_vertex)))
			break ;
		next_vertex = hashtab_lookup(graph->hashtab, prefer_edge->key);
		if (curr_vertex != graph->end \
			&& is_busy_vertex(curr_vertex, prev_vertex->key) != 0)
		{
			del_edge(graph->hashtab, prev_vertex->key, curr_vertex->key);
			del_edge(graph->hashtab, curr_vertex->key, prev_vertex->key);
			curr_vertex->visited = 0;
			return (0);
		}
		if (find_way(graph, next_vertex, curr_vertex))
		{
			edge_change_direction(next_vertex, curr_vertex->key);
			prefer_edge->cost = -1;
			prefer_edge->rev->cost = -1;
			return (1);
		}
	}
	return (0);
}

/*
** to find any intersections that interfere with other ways
** we should go by edge_back edge (first priority)
** vertex must be not visited
** edge must be already busy and have cost == -1
** if in the end it turned out to get to the start, then delete this edge
*/

int				find_way_back(t_graph *graph, t_vertex *curr_vertex, \
								t_vertex *prev_vertex)
{
	t_edge		*temp_edge;
	t_vertex	*next_vertex;

	temp_edge = curr_vertex->edge_back;
	while (temp_edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, temp_edge->key);
		if (next_vertex != prev_vertex && temp_edge->cost == -1)
		{
			if (find_way(graph, next_vertex, curr_vertex))
			{
				del_edge(graph->hashtab, temp_edge->key, curr_vertex->key);
				return (1);
			}
		}
		temp_edge = temp_edge->next;
	}
	return (0);
}

/*
** the Bhandari's algorithm for finding all possible shortest paths
** http://www.macfreek.nl/memory/Disjoint_Path_Finding
*/

int				find_way(t_graph *graph, t_vertex *vertex, \
								t_vertex *prev_vertex)
{
	if (vertex->visited == 1)
		return (0);
	if (vertex == graph->start)
		return (1);
	vertex->visited = 1;
	if (find_way_back(graph, vertex, prev_vertex))
		return (1);
	if (find_way_forward(graph, vertex, prev_vertex))
		return (1);
	return (0);
}
