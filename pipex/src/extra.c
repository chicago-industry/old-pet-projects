/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:17:44 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:39:49 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipe_close(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

int	file_open(char *name, int oflag)
{
	int	fd;

	fd = open(name, oflag, S_IRWXU);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	fork_create(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		error_handle("fork");
	return (pid);
}

void	t_main_content_free(t_main *st_main)
{
	int			i;
	t_command	*head;
	t_command	*temp;

	head = st_main->comm;
	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
	i = 0;
	while (i < st_main->fd_count)
	{
		free(st_main->fd_pipe[i]);
		i++;
	}
	free(st_main->fd_pipe);
	ft_memset(st_main, 0, sizeof(*st_main));
}
