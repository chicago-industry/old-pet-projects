/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_direction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 08:45:33 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 06:29:42 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** after validation all links are stored in vertex->edge_forwd
** so if A-B link, it means A has B in edges and B has a in edges
** we have an undirected graph
** after breadth-first search our vertices has an bfs value
** in Bhandari's algorithm after finding the shortest way we have to make
** this way directed
** by EDGE_CHAGNE_DIRECTION function
** we put an edge from edge_forwd to edge_back in vertex,
** thus making the direction in the graph
*/

void			to_prev_if_no_first(t_edge **from_edge, t_edge **to_edge, \
										char *key)
{
	t_edge		*curr_edge;
	t_edge		*temp_edge;
	t_edge		*prev_edge;

	if (!from_edge || !*from_edge || !to_edge)
		return ;
	curr_edge = *from_edge;
	prev_edge = curr_edge;
	curr_edge = curr_edge->next;
	while (curr_edge)
	{
		if (ft_strcmp(curr_edge->key, key) == 0)
		{
			temp_edge = curr_edge;
			prev_edge->next = curr_edge->next;
			temp_edge->next = *to_edge;
			*to_edge = temp_edge;
			break ;
		}
		prev_edge = prev_edge->next;
		curr_edge = curr_edge->next;
	}
}

void			to_prev_if_first(t_edge **next_edge, t_edge **prev_edge)
{
	t_edge		*temp_edge;

	if (!next_edge || !*next_edge || !prev_edge)
		return ;
	temp_edge = *next_edge;
	*next_edge = (*next_edge)->next;
	temp_edge->next = *prev_edge;
	*prev_edge = temp_edge;
}

void			edge_change_direction(t_vertex *from_edge_vertex, char *key)
{
	t_edge		*next_edge;

	next_edge = from_edge_vertex->edge_forwd;
	if (!next_edge)
		return ;
	if (ft_strcmp(next_edge->key, key) == 0)
		to_prev_if_first(&from_edge_vertex->edge_forwd, \
							&from_edge_vertex->edge_back);
	else
		to_prev_if_no_first(&from_edge_vertex->edge_forwd, \
							&from_edge_vertex->edge_back, key);
}
