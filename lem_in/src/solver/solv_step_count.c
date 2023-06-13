/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_step_count.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkingsbl <pkingsbl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 20:32:36 by pkingsbl          #+#    #+#             */
/*   Updated: 2020/07/10 17:15:58 by pkingsbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Count the number of steps
*/

size_t			sum_steps(t_way *way, int count_way, int num_ants)
{
	size_t		sum;
	int			ant;
	int			ways;

	ant = 1;
	sum = 0;
	if (!way)
		return (1);
	ways = count_way;
	while (ant <= num_ants)
	{
		ant = step_one(way, &ways, ant, num_ants);
		step_two(way);
		sum = sum + is_step(way);
	}
	while (sum_ants_on_ways(way) > 0)
	{
		step_two(way);
		sum = sum + is_step(way);
	}
	return (sum);
}

size_t			is_step(t_way *way)
{
	t_room_way	*p_ant;

	while (way)
	{
		p_ant = way->start_way;
		while (p_ant)
		{
			if (p_ant->ant != 0)
				return (1);
			p_ant = p_ant->next;
		}
		way = way->next;
	}
	return (0);
}

/*
** Amount of ants on the way
*/

size_t			sum_ants_on_ways(t_way *way)
{
	size_t		sum;

	sum = 0;
	while (way)
	{
		sum = sum + way->ant_on_way;
		way = way->next;
	}
	return (sum);
}
