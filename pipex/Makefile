# NAME OF PROGRAM
NAME = pipex

# SETTINGS
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# HEADER
D_H = include
H = pipex.h t_list.h
H := $(H:%.h=$(D_H)/%.h)

# SRC
D_SRC = src
SRC = \
	main.c \
	extra.c \
	parse.c \
	pc_child.c \
	pc_parent.c \
	path_search.c \
	t_list.c
SRC := $(SRC:%.c=$(D_SRC)/%.c)

# OBJ
D_OBJ = .obj
OBJ := $(SRC:$(D_SRC)/%.c=$(D_OBJ)/%.o)

# LIBS
D_LIB := libfuck
D_LIB_H = include
D_LIB_H := $(addprefix $(D_LIB)/, $(D_LIB_H))
LIB := $(addprefix $(D_LIB)/, libfuck.a)

# RULES
all: $(NAME)

$(NAME)::
	@$(MAKE) -sC $(D_LIB)

$(NAME):: $(OBJ)
	@$(CC) $(CCFLAGS) -o $(NAME) $(OBJ) -L./$(D_LIB) -lfuck;
	@echo $(NAME) "compiled"

$(D_OBJ)/%.o: $(D_SRC)/%.c $(H) $(LIB)
	@mkdir -p $(dir $@)
	@echo "[$<] ---> [$@]"
	@$(CC) $(CFLAGS) -I$(D_LIB_H) -I$(D_H) -c $< -o $@

clean:
	@rm -rf $(D_OBJ)
	@$(MAKE) -sC $(D_LIB) clean

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) -sC $(D_LIB) fclean

re: fclean all

.PHONY: all clean fclean re
