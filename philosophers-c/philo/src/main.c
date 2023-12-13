/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 05:41:19 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/21 20:13:41 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	thread_monitor(t_opt *opt)
{
	int			i;
	uint64_t	ms;

	while (1)
	{
		i = 0;
		ms = gettime_ms();
		while (i < opt->num)
		{
			if (ms > opt->philo[i].ts_last_eat + opt->t_die && \
				opt->philo[i].ts_last_eat != 0)
			{
				message(&opt->philo[i], ENUM_DIE);
				return ;
			}
			if (opt->num_eat && opt->num_philo_ate == opt->num)
			{
				pthread_mutex_lock(&opt->mut->message[0]);
				return ;
			}
			usleep(100);
			i++;
		}
	}
}

static void	*thread_job(void *_philo)
{
	t_philo	*philo;

	philo = (t_philo *)_philo;
	philo->ts_last_eat = gettime_ms();
	if (philo->id % 2)
		usleep(15000);
	while (1)
	{
		philo_actions(philo);
	}
	return (NULL);
}

static int	thread_create(t_opt *opt, t_philo *philo)
{
	pthread_t	thid[MAX_PHILO_NUM];
	uint64_t	timestamp;
	int			i;

	i = 0;
	timestamp = gettime_ms();
	while (i < opt->num)
	{
		philo[i].ts_start = timestamp;
		if (pthread_create(&thid[i], NULL, thread_job, &philo[i]) != 0)
		{
			ft_putstr_fd(E_THR_CREATE, STDERR_FILENO);
			return (1);
		}
		pthread_detach(thid[i]);
		i++;
	}
	thread_monitor(opt);
	return (0);
}

int	main(int ac, char *av[])
{
	t_opt		opt;
	t_mutex		mutex;
	t_philo		*philo;

	if (parse(ac, av, &opt) != 0)
		return (1);
	if (init(&opt, &mutex, &philo) != 0)
		return (1);
	if (thread_create(&opt, philo) != 0)
		return (1);
	mutex_destroy(&mutex, opt.num);
	free(philo);
	return (0);
}
