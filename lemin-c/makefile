# COLORS
BLUE = "\e[38;5;69m"
EOC = "\e[0m"

# COMPILER
CC = gcc
# CFLAGS = -Wall -Wextra -Werror

# PROGRAM NAME
NAME_LEMIN = lem-in

# HEADERS
DIR_H = include/

H = lem_in graph error
H := $(addprefix $(DIR_H), $(addsuffix .h, $(H)))

# LIB (LIBFT, GET_NEXT_LINE, PRINTF)
DIR_LIB = lib/
LIB = lib.a
LIB_RULE = $(addsuffix .lib, $(DIR_LIB))

# PROGRAM LEM_IN
DIR_LEMIN := src/
DIR_LEMIN_GRAPH := $(addprefix $(DIR_LEMIN), graph/)
DIR_LEMIN_VALID := $(addprefix $(DIR_LEMIN), validation/)
DIR_LEMIN_SOLVER := $(addprefix $(DIR_LEMIN), solver/)
DIR_LEMIN_DEBUG := debug/

LEMIN_S = main extra
LEMIN_S := $(addsuffix .c, $(LEMIN_S))

LEMIN_GRAPH_S = graph_hashtab \
				graph_vertex \
				graph_edge \
				graph_edge_del \
				graph_extra
LEMIN_GRAPH_S := $(addsuffix .c, $(LEMIN_GRAPH_S))

LEMIN_VALID_S = valid_ants \
				valid_rooms \
				valid_links \
				valid_extra
LEMIN_VALID_S := $(addsuffix .c, $(LEMIN_VALID_S))

LEMIN_SOLVER_S = solv_extra \
				solv_deadlock \
				solv_bellman_ford \
				solv_bellman_ford_extra \
				solv_bfs \
				solv_bfs_extra \
				solv_find_way \
				solv_way_create \
				solv_direction \
				create_room_way \
				create_way \
				solv_step_action \
				solv_step_count
LEMIN_SOLVER_S := $(addsuffix .c, $(LEMIN_SOLVER_S))

LEMIN_LINKING_S = link_edges
LEMIN_LINKING_S := $(addsuffix .c, $(LEMIN_LINKING_S))

LEMIN_DEBUG_S = debug
LEMIN_DEBUG_S := $(addsuffix .c, $(LEMIN_DEBUG_S))

# FILE PATH TO LIB, LEMIN SRC/OBJ FILES
LEMIN_S := $(addprefix $(DIR_LEMIN), $(LEMIN_S)) \
			$(addprefix $(DIR_LEMIN_GRAPH), $(LEMIN_GRAPH_S)) \
			$(addprefix $(DIR_LEMIN_VALID), $(LEMIN_VALID_S)) \
			$(addprefix $(DIR_LEMIN_SOLVER), $(LEMIN_SOLVER_S)) \
			$(addprefix $(DIR_LEMIN_DEBUG), $(LEMIN_DEBUG_S))
LIB := $(addprefix $(DIR_LIB), $(LIB))

all: $(LIB_RULE) $(NAME_LEMIN)

%.lib:
	@$(MAKE) -sC $(DIR_LIB)

$(NAME_LEMIN) : $(LEMIN_S) $(H) $(LIB)
	@$(CC) $(CFLAGS) $(LEMIN_S) $(LIB) -o $(NAME_LEMIN) -I $(DIR_H)
	@echo $(BLUE)"LEMIN COMPILED"$(EOC)

clean:
	@$(MAKE) -sC $(DIR_LIB) clean

fclean: clean
	@$(MAKE) -sC $(DIR_LIB) fclean
	@rm -f $(NAME_LEMIN)

re: fclean all

.PHONY: all clean fclean re