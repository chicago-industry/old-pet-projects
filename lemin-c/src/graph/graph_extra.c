/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 00:52:34 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/18 01:09:17 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

void			*node_malloc(size_t size)
{
	void		*mem;

	mem = malloc(size);
	return (mem);
}

void			free_graph(t_graph *graph)
{
	int			i;

	if (!graph)
		return ;
	i = 0;
	while (i < HASHTAB_SIZE)
	{
		if (graph->hashtab[i])
			free_vertex(&graph->hashtab[i]);
		i++;
	}
}
