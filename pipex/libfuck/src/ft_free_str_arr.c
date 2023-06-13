/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:15:37 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:15:38 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

void	ft_free_str_arr(char ***arr)
{
	int	i;

	i = 0;
	if (!arr || !*arr)
		return ;
	while (arr[0][i])
	{
		free(arr[0][i]);
		arr[0][i] = NULL;
		i++;
	}
	free(arr[0]);
	arr[0] = NULL;
}
