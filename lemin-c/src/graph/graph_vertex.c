/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_vertex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 00:52:22 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/21 01:36:59 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

/*
** free selected memory of linked list
*/

void			free_vertex(t_vertex **head)
{
	t_vertex	*curr_node;

	if (!head)
		return ;
	while (*head)
	{
		curr_node = *head;
		free_edge(&curr_node->edge_forwd);
		free_edge(&curr_node->edge_back);
		*head = (*head)->next;
		ft_strdel(&curr_node->key);
		free(curr_node);
		curr_node = NULL;
	}
}

/*
** create and initialize node T_VERTEX type
*/

t_vertex		*new_vertex(char *key, int x, int y)
{
	t_vertex	*new_node;

	if (!(new_node = node_malloc(sizeof(*new_node))))
		return (NULL);
	if (!(new_node->key = ft_strdup(key)))
		return (NULL);
	new_node->x = x;
	new_node->y = y;
	new_node->bfs = 0;
	new_node->cost = 0;
	new_node->visited = 0;
	new_node->next = NULL;
	new_node->edge_forwd = NULL;
	new_node->edge_back = NULL;
	return (new_node);
}
