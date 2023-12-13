/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 04:36:59 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/21 06:33:35 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	semaphore_init(t_sem *sem, int num)
{
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_MESG);
	sem->forks = sem_open(SEM_FORK, O_CREAT | O_EXCL, S_IRWXU, num);
	if (sem->forks == SEM_FAILED)
		error_handle(E_SEM_OPEN);
	sem->message = sem_open(SEM_MESG, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (sem->message == SEM_FAILED)
		error_handle(E_SEM_OPEN);
}

void	init(t_opt *opt, t_sem *semaphore, t_philo **philo)
{
	int	i;

	*philo = (t_philo *)malloc(sizeof(**philo) * opt->num);
	if (!*philo)
		error_handle(E_MALLOC);
	opt->semaphore = semaphore;
	opt->philo = *philo;
	i = 0;
	while (i < opt->num)
	{
		ft_memcpy(&(*philo)[i], opt, sizeof(**philo));
		(*philo)[i].id = i;
		i++;
	}
	semaphore_init(semaphore, opt->num);
}
