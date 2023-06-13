/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:16:18 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:16:19 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// UNPROTECTED
//
// The strdup() function returns a pointer to a new string which is a duplicate
// of the string s. Memory for the new string is obtained with malloc(3),
// and can be freed with free(3).

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	len;
	char	*tmp;

	i = 0;
	len = ft_strlen(s);
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	while (i < len)
	{
		tmp[i] = s[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}
