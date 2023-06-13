/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 04:38:04 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/17 05:33:19 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// options:
//	- number_of_philosophers
//	- time_to_die
//	- time_to_eat
//	- time_to_sleep
//	- [number_of_times_each_philosopher_must_eat]

static void	error_usage(char *prog)
{
	ft_putstr_fd("Usage:\t", STDERR_FILENO);
	ft_putstr_fd(prog, STDERR_FILENO);
	ft_putstr_fd(" arg1 arg2 arg3 arg4 [arg5]\n", STDERR_FILENO);
	ft_putstr_fd("\targ1: number of philosophers\n", STDERR_FILENO);
	ft_putstr_fd("\targ2: time to die\n", STDERR_FILENO);
	ft_putstr_fd("\targ3: time to eat\n", STDERR_FILENO);
	ft_putstr_fd("\targ4: time to sleep\n", STDERR_FILENO);
	ft_putstr_fd("\targ5: number of times each philosopher must eat\n", \
				STDERR_FILENO);
}

static int	parse_extra(char *av[])
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (ft_isnumber(av[i]) != 1)
		{
			ft_putstr_fd(E_ARG, STDERR_FILENO);
			ft_putstr_fd(av[i], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(E_NOTNUM, STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}

int	parse(int ac, char *av[], t_opt *opt)
{
	if (ac < MIN_ARG_COUNT || ac > MAX_ARG_COUNT)
	{
		error_usage(av[0]);
		return (1);
	}
	if (parse_extra(av) != 0)
		return (1);
	memset(opt, 0, sizeof(*opt));
	opt->num = ft_atoi(av[1]);
	opt->t_die = ft_atoi(av[2]);
	opt->t_eat = ft_atoi(av[3]);
	opt->t_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		opt->num_eat = ft_atoi(av[5]);
	}
	if (opt->num > MAX_PHILO_NUM || opt->t_die < MIN_PHILO_TIME || \
		opt->t_eat < MIN_PHILO_TIME || opt->t_sleep < MIN_PHILO_TIME || \
		opt->num_eat < 0)
	{
		ft_putstr_fd(E_RANGE, STDERR_FILENO);
		return (1);
	}
	return (0);
}
