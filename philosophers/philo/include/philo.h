/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 05:21:15 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/21 20:09:43 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdint.h>

# define MIN_ARG_COUNT	5
# define MAX_ARG_COUNT	6
# define MAX_PHILO_NUM	200
# define MIN_PHILO_TIME	60

# define E_ARG			"Erorr: "
# define E_NOTNUM		"is not a number.\n"
# define E_RANGE		"Error: value or values out of range\n"
# define E_MALLOC		"Error: malloc()\n"
# define E_MUTEX_INIT	"Error: pthread_mutex_init()\n"
# define E_THR_CREATE	"Error: pthread_create()\n"

# define M_FORK			"has taken a fork"
# define M_THINK		"is thinking"
# define M_EAT			"is eating"
# define M_SLEEP		"is sleeping"
# define M_DIE			"died"

typedef enum e_status
{
	ENUM_FORK,
	ENUM_THINK,
	ENUM_EAT,
	ENUM_SLEEP,
	ENUM_DIE
}	t_status;

typedef struct s_mutex
{
	pthread_mutex_t	forks[MAX_PHILO_NUM];
	pthread_mutex_t	message[1];
	pthread_mutex_t	num_philo_ate[1];
}	t_mutex;

typedef struct s_opt
{
	int				num;
	int				num_eat;
	int				num_philo_ate;
	int				t_die;
	int				t_eat;
	int				t_sleep;

	int				id;
	int				left_fork;
	int				right_fork;
	int				num_ate;
	uint64_t		ts_start;
	uint64_t		ts_last_eat;
	struct s_mutex	*mut;
	struct s_opt	*philo;
	struct s_opt	*opt;
} t_opt,	t_philo;

int			parse(int ac, char *av[], t_opt *opt);
int			init(t_opt *opt, t_mutex *mutex, t_philo **philo);
void		mutex_destroy(t_mutex *mutex, int num);

// extra functions
void		philo_actions(t_philo *philo);
uint64_t	gettime_ms(void);
void		sleep_ms(uint64_t time);
void		message(t_philo *philo, t_status stat);

// lib functions
int			ft_atoi(const char *str);
int			ft_isnumber(char *str);
void		ft_putstr_fd(char *s, int fd);
void		*ft_memcpy(void *dest, const void *source, size_t size);

#endif