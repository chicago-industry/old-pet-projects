/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 02:27:08 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/17 19:03:38 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdio.h>
# include <errno.h>
# include <stdlib.h>

# include "../lib/include/libft.h"
# include "../lib/include/ft_printf.h"
# include "../lib/include/get_next_line.h"
# include "graph.h"

# define ERR_VALID			1
# define ERR_BELLF          2
# define ERR_BFS			2
# define ERR_WAY			3

# define E_ANTS				"wrong number of ants or ants doesn't exist\n"

# define E_STDIN			"can't read from stdin\n"

# define E_ROOM				"wrong room\n"
# define E_ROOM_DUP			"the room already exists\n"
# define E_ROOM_NON_EXIST	"missing start/end room\n"
# define E_ROOM_COORDINATE	"wrong room coordinates\n"

# define E_LINK				"wrong link\n"
# define E_LINK_DUP			"the link already exists or is not unidirectional\n"
# define E_FROM_LINK		"can't find the room from which the edge goes\n"
# define E_TO_LINK			"can't find the room to which the edge goes\n"

# define E_GRAPH_LINK		"can't find any link between START and END rooms\n"

# define E_HASHTAB_ADD		"in HASHTAB_ADD function\n"
# define E_IS_ROOM			"in IS_ROOM function\n"
# define E_IS_LINK			"in IS_LINK function\n"
# define E_ADD_EDGE			"in ADD_EDGE function\n"
# define E_ADD_WAY			"in ADD_WAY function or in\n"
# define E_BFS_NEW_E        "in BFS_NEW_E function\n"
# define E_ADD_ROOM_WAY		"in ADD_ROOM_WAY function\n"
# define E_BELLF_LEAD_UP	"in BELLMAN_FORD_LEAD_UP function\n"

#endif
