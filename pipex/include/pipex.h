/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:18:57 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/09 13:33:38 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libfuck.h"
# include "t_list.h"

# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>

# ifndef IS_BONUS
#  define IS_BONUS 1
# endif

# define ARG_COUNT			5
# define FORK_BAD_EXITED	69
# define BONUS_STDIN		"here_doc"
# define PROMPT				"> "

typedef struct s_comm
{
	char			*av;
	struct s_comm	*next;
}	t_command;

typedef struct s_main
{
	char		**env;
	char		*file_in;
	char		*file_out;
	int			**fd_pipe;
	int			fd_count;
	int			status;
	int			bonus_stdin;
	t_command	*comm;
	t_list_set	set;
}	t_main;

// Error functions
void	error_handle(const char *msg);
void	error_usage(char *prog);
void	error_no_command(char *command);

// Extra functions
int		file_open(char *name, int oflag);
void	pipe_close(int *fd);
int		fork_create(void);
void	t_main_content_free(t_main *st_main);

// PC functions
void	job_all_close_pipes(int **fd, int fd_count);
void	job_parent(t_main *st_main);
void	job_child(t_main *st_main, t_command *comm, int z);

void	parse_commands(t_main *st_main, int ac, char *av[]);
void	create_pipes(t_main *st_main, int ac);

char	**path_search(char *av, char *env[]);
void	job_parent(t_main *st_main);

#endif
