/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 02:26:04 by yshawn            #+#    #+#             */
/*   Updated: 2020/07/27 23:49:18 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
# define GRAPH_H

# include "../lib/include/libft.h"
# include "../lib/include/ft_printf.h"
# include "../lib/include/get_next_line.h"

# define HASHTAB_SIZE 65000
# define HASHTAB_MUL 31

typedef struct			s_edge
{
	char				*key;
	int					cost;
	int					double_side;
	struct s_edge		*next;
	struct s_edge		*rev;
}						t_edge;

typedef struct			s_vertex
{
	char				*key;
	int					x;
	int					y;
	int					bfs;
	int					cost;
	int					visited;
	struct s_vertex		*next;
	struct s_edge		*edge_forwd;
	struct s_edge		*edge_back;
}						t_vertex;

typedef struct			s_graph
{
	int					num_ants;
	struct s_vertex		*start;
	struct s_vertex		*end;
	struct s_vertex		**graphtab;
	struct s_vertex		*hashtab[HASHTAB_SIZE];
}						t_graph;

/*
** malloc func for any size struct
*/

void					*node_malloc (size_t size);
void					free_graph(t_graph *graph);

void					hashtab_init(t_vertex **hashtab);
int						hashtab_hash(char *key);
int						hashtab_add(t_vertex **hashtab, char *key, \
										int x, int y);
t_vertex				*hashtab_lookup(t_vertex **hashtab, char *key);

t_vertex				*new_vertex(char *key, int x, int y);
void					free_vertex(t_vertex **head);

int						add_edge(t_vertex *vertex, char *key, int next_prev);
void					add_edge_jump(t_vertex **hashtab, char *from, char *to);
void					del_edge(t_vertex **hashtab, \
									char *key_from, char *key_to);
void					del_edge_extra(t_edge **link, char *key);
int						del_edge_if_first(t_edge **link, char *key);
void					free_edge(t_edge **head);

#endif
