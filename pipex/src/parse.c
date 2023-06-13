/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:42:58 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/08 18:42:59 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// 3:
//	prog, file1, file2
//	./pipex file1 [cmd ... cmd] file2
// 1:
//	multipe conveyor is (N) programs needs (N-1) pipes

#define ARG_REQUIRED	4

void	create_pipes(t_main *st_main, int ac)
{
	int	i;
	int	**fd_pipe;
	int	fd_count;

	fd_count = ac - ARG_REQUIRED;
	fd_pipe = (int **)malloc(sizeof(int *) * fd_count);
	if (!fd_pipe)
	{
		error_handle("malloc");
	}
	i = 0;
	while (i < fd_count)
	{
		fd_pipe[i] = (int *)malloc(sizeof(int) * 2);
		if (!fd_pipe[i])
		{
			error_handle("malloc");
		}
		i++;
	}
	st_main->fd_count = fd_count;
	st_main->fd_pipe = fd_pipe;
}

// Input processing.
// Creating a linked list of commands from arguments.
// File_in is double-valued:
//	- if this is a bonus and is here_doc as argument, will be written LIMITER
//	- otherwise, will be written input file name.

void	parse_commands(t_main *st_main, int ac, char *av[])
{
	t_command	*last;
	int			i;

	i = 0;
	last = NULL;
	st_main->file_in = av[i];
	if (IS_BONUS && ft_strcmp(st_main->file_in, BONUS_STDIN) == 0)
	{
		if (ac < ARG_COUNT + 1)
			error_usage(av[0]);
		i++;
		st_main->file_in = av[i];
		st_main->bonus_stdin = 1;
	}
	i++;
	while (av[i + 1])
	{
		last = t_list_add_back((void *)&st_main->comm, (void *)&last, \
			&st_main->set);
		if (!last)
			error_handle("malloc");
		last->av = av[i];
		i++;
	}
	st_main->file_out = av[i];
}
