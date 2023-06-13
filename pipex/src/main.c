/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:42:52 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/08 18:42:53 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_handle(const char *msg)
{
	if (msg)
		perror(msg);
	exit(EXIT_FAILURE);
}

// ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
// Tantamount to:
// < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2

void	error_usage(char *prog)
{
	ft_putstr_fd("Usage:\n\t", STDERR_FILENO);
	ft_putstr_fd(prog, STDERR_FILENO);
	ft_putstr_fd("\t[from_file] [cmd_1] ... [cmd_n] [to_file]\n", \
		STDERR_FILENO);
	ft_putstr_fd("\t-- equal to --\n", STDERR_FILENO);
	ft_putstr_fd("\t< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2\n", \
		STDERR_FILENO);
	ft_putstr_fd("\n\t-- or -- \n\n\t", STDERR_FILENO);
	ft_putstr_fd(prog, STDERR_FILENO);
	ft_putstr_fd(" [here_doc] [LIMITER] [cmd_1] ... [cmd_n] [to_file]\n", \
		STDERR_FILENO);
	ft_putstr_fd("\t-- equal to --\n", STDERR_FILENO);
	ft_putstr_fd("\tcmd << LIMITER | cmd1 >> file\n", STDERR_FILENO);
	error_handle(NULL);
}

void	error_no_command(char *command)
{
	ft_putstr_fd("No such command: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	main(int ac, char *av[], char *env[])
{
	t_main	st_main;

	if (IS_BONUS)
	{
		if (ac < ARG_COUNT)
			error_usage(av[0]);
	}
	else
	{
		if (ac != ARG_COUNT)
			error_usage(av[0]);
	}
	av++;
	ft_memset(&st_main, 0, sizeof(st_main));
	t_list_init_settings(&st_main.set, &st_main.comm->next, \
		sizeof(*(st_main.comm)));
	parse_commands(&st_main, ac, av);
	create_pipes(&st_main, ac);
	st_main.env = env;
	job_parent(&st_main);
	t_main_content_free(&st_main);
	return (0);
}
