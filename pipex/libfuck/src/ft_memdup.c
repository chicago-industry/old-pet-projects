/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:15:57 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:15:58 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

void	*ft_memdup(const void *source, size_t size)
{
	void	*dest;

	if (!size || !source)
		return (NULL);
	dest = malloc(size);
	if (!dest)
		return (NULL);
	ft_memset(dest, 0, size);
	if (!ft_memcpy(dest, source, size))
		return (NULL);
	return (dest);
}
