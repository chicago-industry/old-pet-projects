/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:15:27 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:15:31 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// The ft_strjoin() finction allocates (with malloc(3)) and returns a “fresh”
// string ending with ’\0’, result of the concatenation of s1 and s2.
// If the allocation fails the function returns NULL

static size_t	ft_strjoin_get_len(char const *s1, char const *s2)
{
	size_t	len;

	len = 0;
	if (s1)
		len += ft_strlen(s1);
	if (s2)
		len += ft_strlen(s2);
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*dest;
	size_t		i;
	size_t		len;

	i = 0;
	if (!s1 && !s2)
		return (NULL);
	len = ft_strjoin_get_len(s1, s2);
	dest = (char *)malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	while (s1 && *s1)
	{
		dest[i] = *s1;
		i++;
		s1++;
	}
	while (s2 && *s2)
	{
		dest[i] = *s2;
		i++;
		s2++;
	}
	dest[i] = '\0';
	return (dest);
}
