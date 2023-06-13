/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solv_bfs_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 21:54:06 by pkingsbl          #+#    #+#             */
/*   Updated: 2020/07/27 20:45:04 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** return the first element of queue and moves queue to the second element
*/

t_queue			*bfs_poll_e(t_queue **head_q, t_queue **last_q)
{
	t_queue		*first_element;

	first_element = *head_q;
	*head_q = (*head_q)->next;
	if (!*head_q)
		*last_q = NULL;
	first_element->next = NULL;
	return (first_element);
}

/*
** add element to the end of queue
*/

t_queue			*bfs_offer_e(t_queue *head_q, t_queue **last_q, char *key)
{
	t_queue		*new_element;

	if (!(new_element = bfs_new_e(key)))
		return (NULL);
	if (!head_q)
	{
		*last_q = new_element;
		return (new_element);
	}
	(*last_q)->next = new_element;
	*last_q = new_element;
	return (head_q);
}

/*
** create a new element for queue
*/

t_queue			*bfs_new_e(char *key)
{
	t_queue		*new_element;

	if (!(new_element = (t_queue *)malloc(sizeof(t_queue))))
		return (NULL);
	if (!(new_element->key = ft_strdup(key)))
		return (NULL);
	new_element->next = NULL;
	return (new_element);
}
