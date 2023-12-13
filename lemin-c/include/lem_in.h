/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 19:48:40 by yshawn            #+#    #+#             */
/*   Updated: 2020/08/12 20:11:11 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include <stdio.h>
# include <errno.h>
# include <stdlib.h>

# include "../lib/include/libft.h"
# include "../lib/include/ft_printf.h"
# include "../lib/include/get_next_line.h"
# include "graph.h"
# include "error.h"

# define START				"##start"
# define END				"##end"
# define EDGE_FORWD			1
# define EDGE_BACK			0
# define INFINITY			0x7FFFFFFF

/*
** Use for Bellman-Ford algorithm
*/
typedef struct			s_bf
{
	int					num_vertices;
	int					num_temp;
	t_vertex			**bftab;
}						t_bf;

/*
** Use for BFS and Bellman-Ford algorithm
*/
typedef struct			s_queue
{
	char				*key;
	struct s_queue		*next;
}						t_queue;

/*
** Linked list of rooms in way, part of t_way
*/
typedef struct			s_room_way
{
	char				*key;
	int					ant;
	int					x;
	int					y;
	struct s_room_way	*next;
	struct s_room_way	*last;
}						t_room_way;

/*
** Linked list of ways
*/
typedef struct			s_way
{
	int					count;
	int					ant_on_way;
	struct s_room_way	*start_way;
	struct s_way		*next;
	struct s_way		*last;
}						t_way;

/*
** INITIALIZATION--------------------------------------------------------------
*/
void					init_structs(t_graph *graph, t_bf *bf, t_way **way);

/*
** VALIDATION------------------------------------------------------------------
*/
int						validation(t_graph *graph);
/*
** Ants validation
*/
void					create_ants(t_graph *graph, char **line);
int						is_valid_ants(char *line);
/*
** Rooms validation
*/
void					create_room(t_graph *graph, char ***arr, char **line);
void					craate_room_start_end(t_graph *graph, char ***arr, \
												char **line, int start_end);
void					is_comment(t_graph *graph, char ***arr, char **line);
int						is_room(t_graph *graph, char ***arr, char **line);
int						is_valid_room(char **arr, char *line);
/*
** Links validation
*/
void					create_link(t_graph *graph, char ***arr, char **line);
int						is_link(t_graph *graph, char ***arr, char **line);
int						is_valid_room_link(char **arr, char *line);
void					is_valid_name_link(t_graph *graph, char ***arr, \
											char **line);
int						is_duplicate_link(t_vertex **hashtab, char *key_from, \
											char *key_to);
/*
** Check for any possible way from START room to END room
*/
int						is_start_end_way(t_vertex **hashtab, t_vertex *vertex, \
											t_vertex *end);

/*
** ALGORITHM-------------------------------------------------------------------
*/
void					solver(t_graph *graph, t_bf *bf, t_way **way);

/*
** Delete edges that lead to deadlock rooms
*/
int						find_deadlock_room(t_graph *graph, t_vertex *vertex, \
											t_vertex *dest, \
											t_vertex *prev_vertex);

/*
** Breadth-first search
*/
void					bfs(t_graph *graph);
void					bfs_value(t_graph *graph, t_vertex *vertex, \
									t_queue **head_q, t_queue **last_q);
void					bfs_clear(t_vertex **hashtab, t_vertex *vertex);
void					re_bfs(t_graph *graph);

t_queue					*bfs_new_e(char *key);
t_queue					*bfs_offer_e(t_queue *head_q, t_queue **last_q, \
										char *key);
t_queue					*bfs_poll_e(t_queue **head_q, t_queue **last_q);

/*
** Bellman-Ford algorithm
*/
void					bellman_ford_num_vertices(t_graph *graph, t_bf *bf, \
													t_vertex *vertex);
void					bellman_ford(t_graph *graph, t_bf *bf);
void					bellman_ford_create_tab(t_graph *graph, t_bf *bf);
void					bellman_ford_init(t_graph *graph, t_bf *bf, \
											t_vertex *vertex);
void					bellman_ford_init_infinity(t_graph *graph, t_bf *bf, \
													t_queue **head_q, \
													t_queue **last_q);
void					bellman_ford_algorithm(t_graph *graph, t_bf *bf);

/*
** Bhandari's algorithm, looking for all possible ways
*/
int						find_way(t_graph *graph, t_vertex *vertex, \
										t_vertex *prev_vertex);
int						find_way_back(t_graph *graph, t_vertex *curr_vertex, \
										t_vertex *prev_vertex);
int						find_way_forward(t_graph *graph, \
											t_vertex *curr_vertex, \
											t_vertex *prev_vertex);

t_edge					*get_prefer_edge(t_graph *graph, t_vertex *vertex, \
											t_vertex *prev_vertex);
int						is_busy_vertex(t_vertex *vertex, char *came);

void					edge_change_direction(t_vertex *from_edge_vertex, \
												char *key);
void					to_prev_if_first(t_edge **next_edge, \
											t_edge **prev_edge);
void					to_prev_if_no_first(t_edge **from_edge, \
							t_edge **to_edge, char *key);

/*
** creating ways
*/
int						create_way(t_graph *graph, t_way **way);
t_vertex				*get_prefer_vertex(t_graph *graph, \
											t_vertex *prev_vertex);
int						is_start_vertex(t_graph *graph, t_way **way, \
											t_vertex *vertex);
int						create_way_extra(t_graph *graph, t_way **way, \
											t_vertex *vertex);

int						add_way(t_way **way);
void					init_way(t_way *way);
void					free_way(t_way **way);

int						add_room_way(t_way *way, t_vertex *vertex);
void					init_room_way(t_room_way *room);
void					free_room_way(t_room_way **head);
/*
** Sum steps on way
*/
size_t					sum_steps(t_way *way, int count_way, int num_ants);
size_t					is_step(t_way *way);
/*
** Launch ants
*/
int						action_ants(t_way *way, int way_count, int ant_count);
size_t					sum_ants_on_ways(t_way *way);
int						step_one(t_way *way, int *ways, int ant, int num_ants);
void					step_two(t_way *way);
int						check_way(t_way *way, int len);
void					print_ants(t_way *way);

/*
** EXTRA-----------------------------------------------------------------------
*/
void					error(char *err_msg, int err_num);
void					clear_visited(t_graph *graph);
void					clear_visited_room(t_graph *graph, t_vertex *vertex, \
											t_vertex *dest, int next_prev);
size_t					ft_str_chars(const char *s, int c);
void					print_ways(t_way *way, char *ways_name);
void					hashtab_print(t_vertex **hashtab);
void					hashtab_print_links(t_vertex **hashtab);
void					print_vertex(t_vertex *blyat);

#endif
