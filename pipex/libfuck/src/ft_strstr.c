/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:16:37 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:16:38 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;

	if (!needle[0])
		return ((char *)haystack);
	while (*haystack)
	{
		i = 0;
		while (needle[i] && needle[i] == haystack[i])
			i++;
		if (!needle[i])
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
