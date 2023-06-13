/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pc_child.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:18:03 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:32:38 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	job_child_execve(char *command, char **env)
{
	char	**program_av;

	program_av = path_search(command, env);
	if (program_av)
	{
		execve(program_av[0], program_av, env);
		perror(program_av[0]);
	}
	else
	{
		error_no_command(command);
	}
	exit(EXIT_FAILURE);
}

static void	job_child_read_stdin(t_main *st_main)
{
	char	*line;
	int		status;

	status = 0;
	dup2(st_main->fd_pipe[0][1], STDOUT_FILENO);
	while (1)
	{
		write(STDIN_FILENO, PROMPT, sizeof(PROMPT) - 1);
		line = ft_getline(STDIN_FILENO, &status);
		if (!line)
		{
			if (status)
			{
				exit (EXIT_FAILURE);
			}
			break ;
		}
		if (ft_strcmp(line, st_main->file_in) == 0)
		{
			break ;
		}
		write(STDOUT_FILENO, line, ft_strlen(line));
		write(STDOUT_FILENO, "\n", 1);
		free(line);
	}
}

// Open a file to write data from the pipeline.

static void	job_child_last_command(t_main *st_main)
{
	int	fd_tmp;

	if (st_main->bonus_stdin)
	{
		fd_tmp = file_open(st_main->file_out, O_WRONLY | O_CREAT | O_APPEND \
			| O_CLOEXEC);
	}
	else
	{
		fd_tmp = file_open(st_main->file_out, O_WRONLY | O_CREAT | O_TRUNC \
			| O_CLOEXEC);
	}
	dup2(fd_tmp, STDOUT_FILENO);
}

// Open file to read data or read from STDIN.

static void	job_child_first_command(t_main *st_main)
{
	int	fd_tmp;

	if (IS_BONUS && st_main->bonus_stdin)
	{
		job_child_read_stdin(st_main);
		job_all_close_pipes(st_main->fd_pipe, st_main->fd_count);
		exit(EXIT_SUCCESS);
	}
	else
	{
		fd_tmp = file_open(st_main->file_in, O_RDONLY | O_CLOEXEC);
		dup2(fd_tmp, STDIN_FILENO);
	}
}

void	job_child(t_main *st_main, t_command *comm, int z)
{
	if (comm == st_main->comm)
	{
		job_child_first_command(st_main);
	}
	else
	{
		dup2(st_main->fd_pipe[z - 1][0], STDIN_FILENO);
	}
	if (!comm->next)
	{
		job_child_last_command(st_main);
	}
	else
	{
		dup2(st_main->fd_pipe[z][1], STDOUT_FILENO);
	}
	job_all_close_pipes(st_main->fd_pipe, st_main->fd_count);
	job_child_execve(comm->av, st_main->env);
}
