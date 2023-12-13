/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 17:57:27 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/28 00:03:58 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			error(char *err_msg, int err_num)
{
	ft_putstr_fd("ERROR", 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(err_msg, 2);
	exit(err_num);
}

size_t			ft_str_chars(const char *s, int c)
{
	size_t		count;
	char		ch;

	count = 0;
	ch = c;
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == ch)
			count++;
		s++;
	}
	return (count);
}

void			init_structs(t_graph *graph, t_bf *bf, t_way **way)
{
	graph->num_ants = 0;
	graph->start = NULL;
	graph->end = NULL;
	graph->graphtab = NULL;
	hashtab_init(graph->hashtab);
	bf->num_vertices = 0;
	bf->num_temp = 0;
	bf->bftab = NULL;
	*way = NULL;
}
