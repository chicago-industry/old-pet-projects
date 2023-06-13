/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_links.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 01:23:51 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 04:26:46 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int				is_duplicate_link(t_vertex **hashtab, char *from, char *to)
{
	t_vertex	*vertex;
	t_edge		*edge;

	if (ft_strcmp(from, to) == 0)
		return (1);
	vertex = hashtab_lookup(hashtab, to);
	if (vertex->edge_forwd)
	{
		edge = vertex->edge_forwd;
		while (edge)
		{
			if (ft_strcmp(edge->key, from) == 0)
				return (1);
			edge = edge->next;
		}
	}
	return (0);
}

int				is_valid_room_link(char **arr, char *line)
{
	int			i;
	char		*symb;

	i = 0;
	symb = NULL;
	if (ft_str_chars(line, '-') != 1)
		return (0);
	while (arr[i])
		i++;
	if (i != 2 || **arr == 'L' || **arr == '#')
		return (0);
	if ((symb = ft_strchr(*arr, '-')) && *symb == '-')
		return (0);
	return (1);
}

void			is_valid_name_link(t_graph *graph, char ***arr, char **line)
{
	t_vertex	*first;
	t_vertex	*second;

	first = NULL;
	second = NULL;
	if (!(first = hashtab_lookup(graph->hashtab, (*arr)[0])))
		error(E_FROM_LINK, ERR_VALID);
	if (!(second = hashtab_lookup(graph->hashtab, (*arr)[1])))
		error(E_TO_LINK, ERR_VALID);
	if (is_duplicate_link(graph->hashtab, (*arr)[0], (*arr)[1]) ||
		is_duplicate_link(graph->hashtab, (*arr)[1], (*arr)[0]))
	{
		error(E_LINK_DUP, ERR_VALID);
	}
	else
	{
		if (!(add_edge(first, (*arr)[1], 1)))
			error(E_ADD_EDGE, ERR_VALID);
		if (!(add_edge(second, (*arr)[0], 1)))
			error(E_ADD_EDGE, ERR_VALID);
		add_edge_jump(graph->hashtab, (*arr)[0], (*arr)[1]);
	}
}

int				is_link(t_graph *graph, char ***arr, char **line)
{
	ft_arrdel((void ***)arr);
	if (!(*arr = ft_strsplit(*line, '-')))
		error(E_IS_LINK, ERR_VALID);
	if (is_valid_room_link(*arr, *line))
		return (1);
	return (0);
}

/*
** read 'links' between rooms
** checks for valid:
** for example:
** 'name X_room-name Y_room'
**
** wrong:
** 'name X_room-name Y_room'
** ...
** ...
** 'name Y_room-name X_room'
*/

void			create_link(t_graph *graph, char ***arr, char **line)
{
	if (!is_link(graph, arr, line))
		error(E_LINK, ERR_VALID);
	is_valid_name_link(graph, arr, line);
	ft_arrdel((void ***)arr);
	while (get_next_line(0, line))
	{
		ft_printf("%s\n", *line);
		if (**line != '#')
		{
			if (!is_link(graph, arr, line))
				error(E_LINK, ERR_VALID);
			is_valid_name_link(graph, arr, line);
			ft_arrdel((void ***)arr);
		}
	}
}
