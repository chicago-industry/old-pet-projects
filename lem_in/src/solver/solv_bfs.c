/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_bfs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 19:21:09 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/27 21:25:22 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			re_bfs(t_graph *graph)
{
	bfs_clear(graph->hashtab, graph->start);
	bfs(graph);
}

void			bfs_clear(t_vertex **hashtab, t_vertex *vertex)
{
	t_edge		*edge;

	if (vertex->visited == 1)
		return ;
	vertex->visited = 1;
	vertex->bfs = 0;
	edge = vertex->edge_back;
	while (edge)
	{
		bfs_clear(hashtab, hashtab_lookup(hashtab, edge->key));
		edge = edge->next;
	}
}

void			bfs_value(t_graph *graph, t_vertex *vertex, \
							t_queue **head_q, t_queue **last_q)
{
	t_edge		*edge;
	t_vertex	*next_vertex;

	edge = vertex->edge_back;
	while (edge)
	{
		next_vertex = hashtab_lookup(graph->hashtab, edge->key);
		if (next_vertex != graph->start && vertex != graph->end \
			&& !next_vertex->bfs)
		{
			next_vertex->bfs = vertex->bfs + 1;
			if (!(*head_q = bfs_offer_e(*head_q, last_q, next_vertex->key)))
				error(E_BFS_NEW_E, ERR_BFS);
		}
		edge = edge->next;
	}
}

/*
** bfs starts from graph->start,
** then all edge's rooms will be added into queue step-by-step
*/

void			bfs(t_graph *graph)
{
	t_queue		*head_q;
	t_queue		*last_q;
	t_queue		*first_q;

	if (!(head_q = bfs_new_e(graph->start->key)))
		error(E_BFS_NEW_E, ERR_BFS);
	last_q = head_q;
	while (head_q)
	{
		first_q = bfs_poll_e(&head_q, &last_q);
		bfs_value(graph, hashtab_lookup(graph->hashtab, first_q->key), \
					&head_q, &last_q);
		ft_strdel(&first_q->key);
		free(first_q);
		first_q = NULL;
	}
}
