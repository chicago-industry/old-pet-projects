/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:17:59 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:50:33 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#define PATH    "PATH="

// This function receives a string like "ls -la", splits into a subset of lines
// by spaces and adds a '/' to the first line of the subset.

// This procedure is necessary in order to then substitute data
// from the PATH environment variable to search for the program
// in the specified system directories.

// As a result we get {"/ ls", "-la", NULL}.

static char	**path_search_make_argv(char *av)
{
	char	**program;
	char	*tmp;

	program = NULL;
	tmp = NULL;
	program = ft_strsplit(av, ' ');
	if (!program)
	{
		error_handle("ft_strsplit");
	}
	tmp = ft_strjoin("/", program[0]);
	if (!tmp)
	{
		error_handle("ft_strjoin");
	}
	free(program[0]);
	program[0] = tmp;
	return (program);
}

// Looking for a program in directories from the PATH environment variable:
//	- if successful, add the directory to the name of the program
//	- on failure, return NULL.

static char	**path_search_extra(char *av, char **splited_path)
{
	int		i;
	char	*tmp;
	char	**program;

	i = 0;
	tmp = NULL;
	program = path_search_make_argv(av);
	while (splited_path[i])
	{
		tmp = ft_strjoin(splited_path[i], program[0]);
		if (!tmp)
		{
			error_handle("ft_strjoin");
		}
		if (access(tmp, F_OK) == 0)
		{
			free(program[0]);
			program[0] = tmp;
			return (program);
		}
		free(tmp);
		i++;
	}
	ft_free_str_arr(&program);
	return (NULL);
}

// If local program (bin/script).

static void	path_search_extra_no_system_bin(char ***splited_av, char *av)
{
	*splited_av = ft_strsplit(av, ' ');
	if (!*splited_av)
	{
		error_handle("ft_strsplit");
	}
}

// If 'system' program (bin/script).
// Signs:
//	- no any '/' in argument name

static void	path_search_extra_system_bin(char ***splited_av, char *av, \
											char *env[])
{
	int		i;
	char	**splited_path;

	i = 0;
	splited_path = NULL;
	while (env[i])
	{
		if (ft_strnstr(env[i], PATH, sizeof(PATH) - 1))
		{
			splited_path = ft_strsplit(env[i] + sizeof(PATH) - 1, ':');
			if (!splited_path)
			{
				error_handle("ft_strsplit");
			}
			*splited_av = path_search_extra(av, splited_path);
			break ;
		}
		i++;
	}
	if (splited_path)
	{
		ft_free_str_arr(&splited_path);
	}
}

char	**path_search(char *av, char *env[])
{
	char	**splited_av;

	splited_av = NULL;
	if (!*av)
	{
		return (NULL);
	}
	if (ft_strchr(av, '/'))
	{
		path_search_extra_no_system_bin(&splited_av, av);
	}
	else
	{
		path_search_extra_system_bin(&splited_av, av, env);
	}
	return (splited_av);
}
