/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_edge_del.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 00:52:47 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 02:07:16 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

/*
** if needed edge is first in linked list
*/

int				del_edge_if_first(t_edge **link, char *key)
{
	t_edge		*copy_link;

	if (!link || !*link)
		return (0);
	copy_link = *link;
	if (copy_link && ft_strcmp(copy_link->key, key) == 0)
	{
		*link = (*link)->next;
		ft_strdel(&copy_link->key);
		free(copy_link);
		return (1);
	}
	return (0);
}

/*
** find needed edge in linked list and delete it from it
*/

void			del_edge_extra(t_edge **link, char *key)
{
	t_edge		*copy_link;
	t_edge		*pre_link;

	if (!link || !*link)
		return ;
	if (del_edge_if_first(link, key))
		return ;
	pre_link = *link;
	copy_link = (*link)->next;
	while (copy_link)
	{
		if (ft_strcmp(copy_link->key, key) == 0)
		{
			pre_link->next = copy_link->next;
			ft_strdel(&copy_link->key);
			free(copy_link);
			return ;
		}
		copy_link = copy_link->next;
		pre_link = pre_link->next;
	}
}

/*
** delete edge between two vertices (rooms)
** key_from is from which vertex edge is goes
** key_to is in which edge is goes
*/

void			del_edge(t_vertex **hashtab, char *key_from, char *key_to)
{
	t_vertex	*from;
	t_vertex	*to;

	from = hashtab_lookup(hashtab, key_from);
	to = hashtab_lookup(hashtab, key_to);
	del_edge_extra(&from->edge_forwd, key_to);
	del_edge_extra(&to->edge_back, key_from);
}
