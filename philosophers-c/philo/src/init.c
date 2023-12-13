/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 04:36:59 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/20 05:48:15 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	mutex_init_extra(pthread_mutex_t *mut, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		if (pthread_mutex_init(&mut[i], NULL) != 0)
		{
			ft_putstr_fd(E_MUTEX_INIT, STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	mutex_init(t_mutex *mutex, int num)
{
	if (mutex_init_extra(mutex->forks, num) != 0)
	{
		return (1);
	}
	if (mutex_init_extra(mutex->message, 1) != 0)
	{
		return (1);
	}
	if (mutex_init_extra(mutex->num_philo_ate, 1) != 0)
	{
		return (1);
	}
	return (0);
}

int	init(t_opt *opt, t_mutex *mutex, t_philo **philo)
{
	int	i;

	*philo = (t_philo *)malloc(sizeof(**philo) * opt->num);
	if (!*philo)
	{
		ft_putstr_fd(E_MALLOC, STDERR_FILENO);
		return (1);
	}
	opt->mut = mutex;
	opt->philo = *philo;
	opt->opt = opt;
	i = 0;
	while (i < opt->num)
	{
		ft_memcpy(&(*philo)[i], opt, sizeof(**philo));
		(*philo)[i].id = i;
		(*philo)[i].left_fork = i;
		(*philo)[i].right_fork = (i + 1) % opt->num;
		i++;
	}
	if (mutex_init(mutex, opt->num) != 0)
	{
		return (1);
	}
	return (0);
}

static void	mutex_destroy_extra(pthread_mutex_t *mut_forks, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&mut_forks[i]);
		i++;
	}
}

void	mutex_destroy(t_mutex *mutex, int num)
{
	mutex_destroy_extra(mutex->forks, num);
	mutex_destroy_extra(mutex->message, 1);
	mutex_destroy_extra(mutex->num_philo_ate, 1);
}
