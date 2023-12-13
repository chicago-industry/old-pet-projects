/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_coord.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 03:37:49 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 06:46:47 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lem_in.h"

int				is_duplicate_coordinates(t_vertex **hashtab)
{
	t_vertex	*current;
	t_vertex	*tmp;

	for (int i = 0; i < HASHTAB_SIZE; i++)
	{
		if (hashtab[i])
		{
			for (current = hashtab[i]; current; current = current->next)
			{
				for (int n = 0; n < HASHTAB_SIZE; n++)
				{
					if (hashtab[n])
					{
						tmp = hashtab[i];
						for (tmp = hashtab[n]; tmp; tmp = tmp->next)
						{
							if (current != tmp && current->x == tmp->x && current->y == tmp->y)
							{
								ft_printf("{yellow}%d %d %d %d\n{eoc}", current->x, current->y, tmp->x, tmp->y);
								return (1);
							}
						}
					}
				}
			}
		}
	}
	return (0);
}
