/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:16:10 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:16:11 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// UNPROTECTED
//
// The strchr() function returns a pointer to the first occurrence
// of the character c in the string s.

char	*ft_strchr(const char *str, int c)
{
	char	ch;

	ch = c;
	while (*str)
	{
		if (*str == ch)
			return ((char *)str);
		str++;
	}
	return (NULL);
}
