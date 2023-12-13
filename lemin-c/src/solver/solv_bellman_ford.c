/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_bellman_ford.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 21:53:51 by pkingsbl          #+#    #+#             */
/*   Updated: 2020/07/27 23:34:23 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			bellman_ford_algorithm(t_graph *graph, t_bf *bf)
{
	t_edge		*edge;
	t_vertex	*vertex;
	int			i;

	i = 0;
	while (bf->bftab[i])
	{
		bf->bftab[i]->visited = 0;
		edge = bf->bftab[i]->edge_forwd;
		while (edge)
		{
			vertex = hashtab_lookup(graph->hashtab, edge->key);
			if (vertex != graph->start && vertex != graph->end \
				&& bf->bftab[i]->cost != INFINITY \
				&& edge->cost + bf->bftab[i]->cost < vertex->cost)
			{
				vertex->cost = edge->cost + bf->bftab[i]->cost;
			}
			edge = edge->next;
		}
		i++;
	}
}

void			bellman_ford_init_infinity(t_graph *graph, t_bf *bf, \
											t_queue **head_q, t_queue **last_q)
{
	t_queue		*first_queue;
	t_edge		*edge;
	t_vertex	*next_vertex;
	t_vertex	*vertex;

	first_queue = bfs_poll_e(head_q, last_q);
	vertex = hashtab_lookup(graph->hashtab, first_queue->key);
	bf->bftab[bf->num_temp++] = vertex;
	vertex->visited = 1;
	edge = vertex->edge_forwd;
	while (edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, edge->key);
		if (next_vertex->visited == 0 && next_vertex != graph->start)
		{
			next_vertex->cost = INFINITY;
			next_vertex->visited = 1;
			if (!(*head_q = bfs_offer_e(*head_q, last_q, next_vertex->key)))
				error(E_BFS_NEW_E, ERR_BFS);
		}
		edge = edge->next;
	}
	ft_strdel(&first_queue->key);
	free(first_queue);
	first_queue = NULL;
}

void			bellman_ford_init(t_graph *graph, t_bf *bf, t_vertex *vertex)
{
	t_queue		*head_q;
	t_queue		*last_q;

	if (!(head_q = bfs_new_e(graph->start->key)))
		error(E_BFS_NEW_E, ERR_BFS);
	last_q = head_q;
	vertex->visited = 1;
	while (head_q)
		bellman_ford_init_infinity(graph, bf, &head_q, &last_q);
	while (bf->num_temp < bf->num_vertices)
	{
		bf->bftab[bf->num_temp] = NULL;
		bf->num_temp++;
	}
}

void			bellman_ford_create_tab(t_graph *graph, t_bf *bf)
{
	if (bf->bftab)
		free(bf->bftab);
	if (!(bf->bftab = (t_vertex **)malloc(sizeof(t_vertex *) \
						* (bf->num_vertices + 1))))
		error(E_BELLF_LEAD_UP, ERR_BELLF);
	bf->bftab[bf->num_vertices] = NULL;
}

void			bellman_ford(t_graph *graph, t_bf *bf)
{
	bellman_ford_create_tab(graph, bf);
	bf->num_temp = 0;
	bellman_ford_init(graph, bf, graph->start);
	bellman_ford_algorithm(graph, bf);
}
