/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 06:02:56 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/20 05:48:15 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(&philo->mut->forks[philo->left_fork]);
	}
	else
	{
		pthread_mutex_lock(&philo->mut->forks[philo->right_fork]);
	}
	message(philo, ENUM_FORK);
	if (philo->id % 2)
	{
		pthread_mutex_lock(&philo->mut->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_lock(&philo->mut->forks[philo->left_fork]);
	}
	message(philo, ENUM_FORK);
}

static void	put_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_unlock(&philo->mut->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->mut->forks[philo->left_fork]);
	}
	if (philo->id % 2)
	{
		pthread_mutex_unlock(&philo->mut->forks[philo->left_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->mut->forks[philo->right_fork]);
	}
}

static void	eat(t_philo *philo)
{
	philo->ts_last_eat = gettime_ms();
	message(philo, ENUM_EAT);
	philo->num_ate++;
	if (philo->num_eat && philo->num_ate == philo->num_eat)
	{
		pthread_mutex_lock(&philo->mut->num_philo_ate[0]);
		philo->opt->num_philo_ate++;
		pthread_mutex_unlock(&philo->mut->num_philo_ate[0]);
	}
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
