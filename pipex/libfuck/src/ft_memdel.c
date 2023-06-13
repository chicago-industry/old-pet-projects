/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:15:53 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:15:54 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

// The function takes as a parameter the address of a memory area that needs
// to be freed with free(), then puts the pointer to NULL.

void	ft_memdel(void **ap)
{
	if (!ap || !*ap)
	{
		return ;
	}
	else
	{
		free(*ap);
		*ap = NULL;
	}
}
