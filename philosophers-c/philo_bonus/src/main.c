/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 05:41:19 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/21 06:31:03 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*thread_monitor(void *_philo)
{
	uint64_t	ms;
	t_philo		*philo;

	philo = (t_philo *)_philo;
	while (1)
	{
		ms = gettime_ms();
		if (ms > philo->ts_last_eat + philo->t_die && \
			philo->ts_last_eat != 0)
		{
			message(philo, ENUM_DIE);
			exit(10);
		}
		usleep(100);
	}
	return (NULL);
}

static void	child_job(t_philo *philo)
{
	pthread_t	thid;

	philo->ts_last_eat = gettime_ms();
	if (pthread_create(&thid, NULL, thread_monitor, philo) != 0)
		error_handle(E_THR_CREATE);
	pthread_detach(thid);
	if (philo->id % 2)
		usleep(15000);
	while (1)
	{
		philo_actions(philo);
		if (philo->num_eat && philo->num_ate >= philo->num_eat)
			exit(0);
	}
}

static void	parent_wait(int num)
{
	int	i;
	int	status;

	i = 0;
	while (i < num)
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

static void	parent_job(t_opt *opt, t_philo *philo)
{
	int			pid[MAX_PHILO_NUM];
	uint64_t	timestamp;
	int			i;

	i = 0;
	timestamp = gettime_ms();
	while (i < opt->num)
	{
		philo[i].ts_start = timestamp;
		pid[i] = fork();
		if (pid[i] == -1)
			error_handle(E_FORK);
		else if (pid[i] == 0)
			child_job(&philo[i]);
		i++;
	}
	parent_wait(opt->num);
}

int	main(int ac, char *av[])
{
	t_opt		opt;
	t_sem		semaphore;
	t_philo		*philo;

	parse(ac, av, &opt);
	init(&opt, &semaphore, &philo);
	parent_job(&opt, philo);
	sem_close(semaphore.forks);
	sem_close(semaphore.message);
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_MESG);
	free(philo);
	return (0);
}
