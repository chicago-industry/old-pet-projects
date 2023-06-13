/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 04:36:15 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/17 06:10:45 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

void	*ft_memcpy(void *dest, const void *source, size_t size)
{
	unsigned char	*dest_copy;
	unsigned char	*source_copy;

	if (!size || dest == source)
		return (NULL);
	dest_copy = (unsigned char *)dest;
	source_copy = (unsigned char *)source;
	while (size)
	{
		*dest_copy = *source_copy;
		dest_copy++;
		source_copy++;
		size--;
	}
	return (dest);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if ((str[i] == '-' || str[i] == '+'))
		{
			if (i == 0)
			{
				i++;
				if (str[i])
					continue ;
				else
					return (0);
			}
			else
				return (0);
		}
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}
