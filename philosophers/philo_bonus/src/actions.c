/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 06:02:56 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/21 06:33:51 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->semaphore->forks);
	message(philo, ENUM_FORK);
	sem_wait(philo->semaphore->forks);
	message(philo, ENUM_FORK);
}

static void	put_forks(t_philo *philo)
{
	sem_post(philo->semaphore->forks);
	sem_post(philo->semaphore->forks);
}

static void	eat(t_philo *philo)
{
	philo->ts_last_eat = gettime_ms();
	message(philo, ENUM_EAT);
	philo->num_ate++;
	sleep_ms(philo->t_eat);
}

static void	sleep_think(t_philo *philo)
{
	message(philo, ENUM_SLEEP);
	sleep_ms(philo->t_sleep);
	message(philo, ENUM_THINK);
}

void	philo_actions(t_philo *philo)
{
	take_forks(philo);
	eat(philo);
	put_forks(philo);
	sleep_think(philo);
}
