/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkingsbl <pkingsbl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 20:30:47 by pkingsbl          #+#    #+#             */
/*   Updated: 2020/07/10 17:15:06 by pkingsbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Movements ants from start to end room
*/

int				action_ants(t_way *way, int count_way, int num_ants)
{
	int			ant;
	int			ways;

	ant = 1;
	if (!way)
		return (1);
	ways = count_way;
	while (ant <= num_ants)
	{
		ant = step_one(way, &ways, ant, num_ants);
		step_two(way);
		print_ants(way);
	}
	while (sum_ants_on_ways(way) > 0)
	{
		step_two(way);
		print_ants(way);
	}
	return (0);
}

/*
** Movement ants from start
*/

int				step_one(t_way *way, int *ways, int ant, int num_ants)
{
	int			tmp_w;
	t_way		*start_way;

	tmp_w = 0;
	start_way = way;
	while (tmp_w < *ways && way && way->start_way && ant <= num_ants)
	{
		if ((num_ants - (ant - 1)) <= check_way(start_way, way->count))
			*ways = tmp_w > 0 ? tmp_w : 1;
		else if (way->start_way->ant == 0)
		{
			way->start_way->ant = ant;
			way->ant_on_way++;
			ant++;
		}
		tmp_w++;
		way = way->next;
	}
	return (ant);
}

/*
** Movements the ants one step
*/

void			step_two(t_way *way)
{
	int			name;
	int			tmp;
	t_room_way	*p_room;

	while (way)
	{
		name = 0;
		tmp = 0;
		p_room = way->start_way;
		if (way->ant_on_way == 0)
			way = way->next;
		else
		{
			while (p_room)
			{
				tmp = p_room->ant;
				if (!p_room->next && tmp != 0)
					way->ant_on_way--;
				p_room->ant = name;
				name = tmp;
				p_room = p_room->next;
			}
			way = way->next;
		}
	}
}

/*
** Check movement ant by this way
*/

int				check_way(t_way *way, int len)
{
	int			res;

	res = 0;
	while (way && way->count < len)
	{
		res = res + (len - way->count);
		way = way->next;
	}
	return (res);
}

/*
** Print movements ants in rooms
*/

void			print_ants(t_way *way)
{
	int			f;
	t_room_way	*p_ant;

	f = 0;
	while (way)
	{
		p_ant = way->start_way;
		while (p_ant)
		{
			if (p_ant->ant != 0)
			{
				ft_printf("L%d-%s ", p_ant->ant, p_ant->key);
				f = 1;
			}
			p_ant = p_ant->next;
		}
		way = way->next;
	}
	if (f)
		write(1, "\n", 1);
}
