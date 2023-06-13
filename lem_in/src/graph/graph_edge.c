/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_edge.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 00:52:47 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 02:08:02 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

/*
** free selected memory of linked list
*/

void			free_edge(t_edge **head)
{
	t_edge		*curr_node;

	if (!head)
		return ;
	while (*head)
	{
		curr_node = *head;
		*head = (*head)->next;
		ft_strdel(&curr_node->key);
		free(curr_node);
		curr_node = NULL;
	}
}

/*
** create and add the T_EDGE type new node to start of linked list
*/

int				add_edge(t_vertex *vertex, char *key, int next_prev)
{
	t_edge		*new_node;

	if (!vertex)
		return (0);
	if (!(new_node = node_malloc(sizeof(*new_node))))
		return (0);
	new_node->cost = 1;
	new_node->double_side = 0;
	new_node->rev = NULL;
	if (!(new_node->key = ft_strdup(key)))
		return (0);
	if (next_prev)
	{
		new_node->next = vertex->edge_forwd;
		vertex->edge_forwd = new_node;
	}
	else
	{
		new_node->next = vertex->edge_back;
		vertex->edge_back = new_node;
	}
	return (1);
}

/*
** suppose we have two rooms 'A' and 'B' (vertices)
** and there is connection (edge) from 'A' room to 'B' room
** 'A' will have in A_NEXT_edges 'B', and B_room will have in B_PREV_edges 'A'
** sometimes we need to apply to the same edge from other side
** this fuction makes possible to allows to jump between tho edges:
** (one is A_next_edges, second is B_PREV_edges)
*/

void			add_edge_jump(t_vertex **hashtab, char *from, char *to)
{
	t_vertex	*v_from;
	t_vertex	*v_to;

	v_from = hashtab_lookup(hashtab, from);
	v_to = hashtab_lookup(hashtab, to);
	v_from->edge_forwd->rev = v_to->edge_forwd;
	v_to->edge_forwd->rev = v_from->edge_forwd;
}
