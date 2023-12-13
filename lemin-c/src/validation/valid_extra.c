/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 01:23:51 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 02:06:51 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** check for any possible way from start room to end room
*/

int				is_start_end_way(t_vertex **hashtab, t_vertex *vertex, \
									t_vertex *end)
{
	t_edge		*edge;

	if (vertex->visited == 1)
		return (0);
	else
		vertex->visited = 1;
	if (vertex == end)
		return (1);
	edge = vertex->edge_forwd;
	while (edge)
	{
		if (is_start_end_way(hashtab, hashtab_lookup(hashtab, edge->key), end))
			return (1);
		edge = edge->next;
	}
	return (0);
}
