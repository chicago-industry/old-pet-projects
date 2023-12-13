/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_hashtab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkingsbl <pkingsbl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 00:52:29 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/10 17:13:49 by pkingsbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

/*
** return the found node using hashtab_hash func
*/

t_vertex		*hashtab_lookup(t_vertex **hashtab, char *key)
{
	int			index;
	t_vertex	*node;

	index = hashtab_hash(key);
	node = hashtab[index];
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

/*
** encrypt the key
*/

int				hashtab_hash(char *key)
{
	unsigned long long	h;
	char				*p;

	h = 0;
	p = key;
	while (*p)
	{
		h = (h * HASHTAB_MUL) ^ (int)*p;
		p++;
		h++;
	}
	return ((h & 0x7FFFFFFF) % HASHTAB_SIZE);
}

/*
** creating a struct of T_VERTEX type
** and using hashtab_hash func adding it to hashtab[HASHSIZE]
*/

int				hashtab_add(t_vertex **hashtab, char *key, int x, int y)
{
	t_vertex			*node;
	unsigned long long	index;

	index = hashtab_hash(key);
	if (!(node = new_vertex(key, x, y)))
		return (1);
	node->next = hashtab[index];
	hashtab[index] = node;
	return (0);
}

/*
** assign NULL to each hashtab[i] element
*/

void			hashtab_init(t_vertex **hashtab)
{
	int			i;

	i = 0;
	while (i < HASHTAB_SIZE)
	{
		hashtab[i] = NULL;
		i++;
	}
}
