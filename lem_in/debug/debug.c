#include "../include/lem_in.h"

// print_ways(*way_orig, "ORIGINALS:");
// print_ways(*way_clean, "CLEAN:");
// ft_printf("kol-vo pytei original: %lu\n", way_o);
// ft_printf("%d\n", sum_steps(*way_orig, way_o, graph->num_ants));
// ft_printf("kol-vo pytei clean: %lu\n", way_c);
// ft_printf("%d\n", sum_steps(*way_clean, way_c, graph->num_ants));

void			print_ways(t_way *way, char *ways_name)
{
	t_room_way	*room_way;

	ft_printf("{green}%s{eoc}\n", ways_name);
	while (way)
	{
		ft_printf("length: %d\n", (way)->count);
		room_way = way->start_way;
		while (room_way)
		{
			ft_printf("%s\n", room_way->key);
			room_way = room_way->next;
		}
		way = way->next;
	}
}

void			print_vertex(t_vertex *blyat)
{
	ft_printf("{yellow}room: {green}%s\n{eoc}bellfd = %ld\nbfs: %d\n%visided = %d\n{eoc}", blyat->key, blyat->cost, blyat->bfs, blyat->visited);
	t_edge *tmp_edge;
	t_edge *tmp_edge_back;

	tmp_edge = blyat->edge_forwd;
	tmp_edge_back = blyat->edge_back;
	ft_printf("путь вперед : ");
	while (tmp_edge)
	{
		ft_printf("%s", tmp_edge->key);
		ft_printf("{blue}(%d){eoc} - ", tmp_edge->cost);
		tmp_edge = tmp_edge->next;
	}
	ft_printf("\nпуть назад  : ");
	while (tmp_edge_back)
	{
		ft_printf("%s", tmp_edge_back->key);
		ft_printf("{blue}(%d){eoc} - ", tmp_edge_back->cost);
		tmp_edge_back = tmp_edge_back->next;
	}
	ft_printf("\n");
}

void			hashtab_print(t_vertex **hashtab)
{
	t_vertex *node;

	node = NULL;
	for (int i = 0; i < HASHTAB_SIZE; i++)
	{
		node = hashtab[i];
		if (node != NULL)
		{
			while (1)
			{
				while (node)
				{
					if (node != NULL)
					{
						ft_printf("{green}%s{eoc} -> ", node->key);
					}
					node = node->next;
				}
				ft_printf("\n");
				break;
			}
		}
	}
}

void			hashtab_print_links(t_vertex **hashtab)
{
	t_vertex	*blyat;

	for (int i = 0; i < HASHTAB_SIZE; i++)
	{
		blyat = hashtab[i];
		if (blyat != NULL)
		{
			while (blyat)
			{
				if (!blyat->edge_forwd && !blyat->edge_back)
				{
					blyat = blyat->next;
					continue;
				}
				ft_printf("{yellow}room: {green}%s\n{eoc}bf = {blue}%ld\n{eoc}vis: %d\n{eoc}", blyat->key, blyat->cost, blyat->visited);
				ft_printf("bfs: {blue}%d\n{eoc}", blyat->bfs);
				t_edge *tmp_edge;
				t_edge *tmp_edge_back;

				tmp_edge = blyat->edge_forwd;
				tmp_edge_back = blyat->edge_back;
				ft_printf("путь вперед : ");
				while (tmp_edge)
				{
					ft_printf("%s", tmp_edge->key);
                    ft_printf("{blue}(%d){eoc}", tmp_edge->cost);
					tmp_edge = tmp_edge->next;
				}
				ft_printf("\nпуть назад  : ");
				while (tmp_edge_back)
				{
					ft_printf("%s", tmp_edge_back->key);
					ft_printf("{blue}(%d){eoc}", tmp_edge_back->cost);
					tmp_edge_back = tmp_edge_back->next;
				}
				ft_printf("\n");
				blyat = blyat->next;
			}
		}
	}
}