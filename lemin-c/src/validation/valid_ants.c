/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_ants.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 01:23:51 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/14 03:59:53 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** check number of ants for valid (must be < 0 ants < 0x7FFFFFFF)
*/

int				is_valid_ants(char *line)
{
	int			count;

	count = 0;
	if (ft_isnumber_int(line))
		count = ft_atoi(line);
	return (count > 0 ? count : 0);
}

/*
** read number of ants to graph->num_ants if number of ants is valid
*/

void			create_ants(t_graph *graph, char **line)
{
	if (get_next_line(0, line))
	{
		ft_printf("%s\n", *line);
		if (!(graph->num_ants = is_valid_ants(*line)))
			error(E_ANTS, ERR_VALID);
	}
	else
		error(E_STDIN, ERR_VALID);
}
