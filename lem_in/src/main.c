/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 04:45:53 by yshawn            #+#    #+#             */
/*   Updated: 2020/08/12 20:14:52 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** choice best version of ways
** everytime we find new way/ways in find_way
** we save this version into temp_way and compare temp_way with way
** if temp_way is better than way, we save temp_way into way and free temp_way
*/

void			choice_best_version_of_way(t_graph *graph, t_way **way)
{
	t_way		*temp_way;
	int			temp_len;
	static int	len;

	temp_way = NULL;
	if (!*way)
		len = create_way(graph, way);
	else
	{
		temp_len = create_way(graph, &temp_way);
		if (sum_steps(temp_way, temp_len, graph->num_ants) \
			< sum_steps(*way, len, graph->num_ants))
		{
			free_way(way);
			*way = temp_way;
			len = temp_len;
		}
		else
			free_way(&temp_way);
	}
}

/*
** delete deadlocks
** bellman-ford algorithm
** bhandari algorithm
** bfs for founded ways (to add in t_way *way from shortest to biggest ways)
** create ways
** print the movement of ants
*/

void			solver(t_graph *graph, t_bf *bf, t_way **way)
{
	int			len;

	bellman_ford(graph, bf);
	while (find_way(graph, graph->end, graph->end))
	{
		clear_visited(graph);
		bellman_ford(graph, bf);
		re_bfs(graph);
		clear_visited(graph);
		choice_best_version_of_way(graph, way);
		clear_visited(graph);
	}
	ft_printf("\n");
	action_ants(*way, (*way)->count, graph->num_ants);
}

/*
** delete all edges to deadlock vertices (rooms)
** count of actual vertices if graph (for bellman-ford table)
*/

void			pre_solver(t_graph *graph, t_bf *bf)
{
	while (find_deadlock_room(graph, graph->start, graph->end, graph->start))
		clear_visited(graph);
	clear_visited(graph);
	bellman_ford_num_vertices(graph, bf, graph->start);
	clear_visited(graph);
}

/*
** ckeck num of ants
** check room for valid
** check links between rooms for valid
** check any possible limk between start room and end room
*/

int				validation(t_graph *graph)
{
	char		**arr;
	char		*line;

	arr = NULL;
	line = NULL;
	create_ants(graph, &line);
	create_room(graph, &arr, &line);
	if (!graph->start || !graph->end)
		error(E_ROOM_NON_EXIST, ERR_VALID);
	create_link(graph, &arr, &line);
	if (!(is_start_end_way(graph->hashtab, graph->start, graph->end)))
		error(E_GRAPH_LINK, ERR_VALID);
	clear_visited(graph);
	ft_strdel(&line);
	return (0);
}

int				main(int ac, char **av)
{
	t_graph		graph[1];
	t_bf		bf[1];
	t_way		*way;

	if (ac != 1 && *(av + 1))
	{
		ft_printf("{orange}Usage: no parameters, only from stdin{eoc}\n.");
		ft_printf("{yellow}/lem-in < path/to/map{eoc}\n");
		return (0);
	}
	init_structs(graph, bf, &way);
	validation(graph);
	pre_solver(graph, bf);
	solver(graph, bf, &way);
	free(bf->bftab);
	free_way(&way);
	free_graph(graph);
	return (0);
}
