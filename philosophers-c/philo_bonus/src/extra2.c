/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 05:42:18 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/21 20:11:53 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	gettime_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	sleep_ms(uint64_t time)
{
	uint64_t	ms;

	ms = gettime_ms();
	while (1)
	{
		if (gettime_ms() - ms >= time)
			break ;
		usleep(100);
	}
}

static char	*message_extra(t_status stat)
{
	if (stat == ENUM_FORK)
		return (M_FORK);
	else if (stat == ENUM_THINK)
		return (M_THINK);
	else if (stat == ENUM_EAT)
		return (M_EAT);
	else if (stat == ENUM_SLEEP)
		return (M_SLEEP);
	return (M_DIE);
}

void	message(t_philo *philo, t_status stat)
{
	uint64_t	ms;

	ms = gettime_ms();
	ms = ms - philo->ts_start;
	sem_wait(philo->semaphore->message);
	printf("%llu %d %s\n", ms, philo->id + 1, message_extra(stat));
	if (stat == ENUM_DIE)
		return ;
	sem_post(philo->semaphore->message);
}

int	ft_atoi(const char *str)
{
	size_t				i;
	unsigned long long	n;
	int					neg;

	i = 0;
	n = 0;
	neg = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return ((int)n * neg);
}
