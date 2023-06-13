/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:16:01 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:16:03 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// UNPROTECTED
//
// The memset() function fills the first n bytes of the memory area
// pointed to by s with the constant byte c.

void	*ft_memset(void *s, int c, size_t len)
{
	size_t	i;
	char	*temp;

	temp = (char *)s;
	i = 0;
	while (i < len)
	{
		temp[i] = c;
		i++;
	}
	return (s);
}
