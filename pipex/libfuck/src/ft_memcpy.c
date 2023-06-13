/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m1c <m1c@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:12:27 by m1c               #+#    #+#             */
/*   Updated: 2021/11/07 15:12:29 by m1c              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// The function copies n bytes
// from memory area src to memory area dest.
// The memory areas should not overlap.
// Use memmove() if the memory areas do overlap.

void	*ft_memcpy(void *dest, const void *source, size_t size)
{
	char	*dest_copy;
	char	*source_copy;

	if (!size || dest == source)
		return (NULL);
	dest_copy = (char *)dest;
	source_copy = (char *)source;
	while (size)
	{
		*dest_copy = *source_copy;
		dest_copy++;
		source_copy++;
		size--;
	}
	return (dest);
}
