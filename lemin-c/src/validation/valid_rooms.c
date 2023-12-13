/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_rooms.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 01:23:51 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 04:24:30 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int				is_valid_room(char **arr, char *line)
{
	int			i;

	i = 0;
	if (ft_str_chars(line, ' ') != 2)
		return (0);
	while (arr[i])
		i++;
	if (i != 3 || **arr == 'L')
		return (0);
	if (!(ft_isnumber_int(arr[1])) || !(ft_isnumber_int(arr[2])))
		return (0);
	return (1);
}

void			craate_room_start_end(t_graph *graph, char ***arr, \
										char **line, int start_end)
{
	if (get_next_line(0, line))
	{
		ft_printf("%s\n", *line);
		if (!(is_room(graph, arr, line)))
			error(E_ROOM, ERR_VALID);
		if (start_end)
			graph->start = hashtab_lookup(graph->hashtab, (*arr)[0]);
		else
			graph->end = hashtab_lookup(graph->hashtab, (*arr)[0]);
		ft_arrdel((void ***)arr);
	}
}

int				is_room(t_graph *graph, char ***arr, char **line)
{
	int			x;
	int			y;

	x = 0;
	y = 0;
	if (!(*arr = ft_strsplit(*line, ' ')))
		error(E_IS_ROOM, ERR_VALID);
	if (is_valid_room(*arr, *line))
	{
		x = ft_atoi((*arr)[1]);
		y = ft_atoi((*arr)[2]);
		if (x < 0 || y < 0)
			error(E_ROOM_COORDINATE, ERR_VALID);
		if (hashtab_lookup(graph->hashtab, (*arr)[0]) != NULL)
			error(E_ROOM_DUP, ERR_VALID);
		if ((hashtab_add(graph->hashtab, (*arr)[0], x, y) != 0))
			error(E_HASHTAB_ADD, ERR_VALID);
		return (1);
	}
	return (0);
}

void			is_comment(t_graph *graph, char ***arr, char **line)
{
	if (ft_strcmp(*line, START) == 0 && graph->start)
		error(E_ROOM_DUP, ERR_VALID);
	if (ft_strcmp(*line, END) == 0 && graph->end)
		error(E_ROOM_DUP, ERR_VALID);
	if (ft_strcmp(*line, START) == 0)
		craate_room_start_end(graph, arr, line, 1);
	else if (ft_strcmp(*line, END) == 0)
		craate_room_start_end(graph, arr, line, 0);
}

/*
** read 'rooms' (example: "name X_coordinate Y_coordinate")
** checks for valid:
** the room's name must not starts from '#' or 'L'
** the room's coordinates must always be integers. (0 - 0x7FFFFFFF)
** must be '##start' room
** must be '##end' room
*/

void			create_room(t_graph *graph, char ***arr, char **line)
{
	while (get_next_line(0, line))
	{
		ft_printf("%s\n", *line);
		if (**line == '#')
			is_comment(graph, arr, line);
		else if (**line != '#')
		{
			if (!(is_room(graph, arr, line)))
			{
				if (is_link(graph, arr, line))
					break ;
				else
					error(E_ROOM, ERR_VALID);
			}
			ft_arrdel((void ***)arr);
		}
	}
	ft_arrdel((void ***)arr);
}
