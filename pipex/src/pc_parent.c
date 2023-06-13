/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pc_parent.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:41:17 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/08 18:41:19 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	job_all_close_pipes(int **fd, int fd_count)
{
	int	i;

	i = 0;
	while (i < fd_count)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

static void	job_parent_open_pipes(int **fd_pipe, int fd_count)
{
	int	i;

	i = 0;
	while (i < fd_count)
	{
		if (pipe(fd_pipe[i]) == -1)
			error_handle("pipe");
		i++;
	}
}

// If WIFEXITED: exited normally.
// Else if WIFSIGNALED: exited because of signal.
// Else: neither exited normally not because of a signal.

static void	job_parent_wait(int fd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < fd_count)
	{
		wait(&status);
		if (status != 0)
		{
			if (WIFEXITED(status))
			{
				exit(WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status))
			{
				exit(WTERMSIG(status));
			}
			else
			{
				exit(FORK_BAD_EXITED);
			}
		}
		i++;
	}
}

void	job_parent(t_main *st_main)
{
	int			i_comm;
	int			pid;
	t_command	*temp;

	i_comm = 0;
	temp = st_main->comm;
	job_parent_open_pipes(st_main->fd_pipe, st_main->fd_count);
	while (temp)
	{
		pid = fork_create();
		if (pid == 0)
		{
			job_child(st_main, temp, i_comm);
		}
		temp = temp->next;
		i_comm++;
	}
	job_all_close_pipes(st_main->fd_pipe, st_main->fd_count);
	job_parent_wait(st_main->fd_count);
}
