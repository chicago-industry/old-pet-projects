/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:16:29 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:16:30 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	k;

	if (!needle)
		return ((char *)haystack);
	k = 0;
	while (haystack && haystack[k] && k < len)
	{
		i = 0;
		while (needle[i] && haystack[k + i] \
			&& needle[i] == haystack[k + i] && k + i < len)
			i++;
		if (!needle[i])
			return ((char *)&haystack[k]);
		k++;
	}
	return (NULL);
}
