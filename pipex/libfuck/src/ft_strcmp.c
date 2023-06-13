/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:16:14 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:16:15 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// UNPROTECTED
//
// The strcmp() function compares the two strings s1_1 and s2_2.
// It returns an integer less than, equal to,
// or greater than zero if s1_1 is found, respectively, to be less than,
// to match, or be greater than s2_2.

int	ft_strcmp(const char *s1_1, const char *s2_2)
{
	char	*s1;
	char	*s2;

	s1 = (char *)s1_1;
	s2 = (char *)s2_2;
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
