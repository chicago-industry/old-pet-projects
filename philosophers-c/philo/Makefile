# NAME OF PROGRAM
NAME = philo

# SETTINGS
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIB = -lpthread

# HEADER
D_H = include
H = philo.h
H := $(H:%.h=$(D_H)/%.h)

# SRC
D_SRC = src
SRC = \
	main.c \
	parse.c \
	init.c \
	actions.c \
	extra.c \
	extra2.c
SRC := $(SRC:%.c=$(D_SRC)/%.c)

# OBJ
D_OBJ = .obj
OBJ := $(SRC:$(D_SRC)/%.c=$(D_OBJ)/%.o)

# RULES
all: $(NAME)

$(NAME):: $(OBJ)
	@$(CC) $(CCFLAGS)  $(OBJ) $(LIB) -o $(NAME)
	@echo $(NAME) "compiled"

$(D_OBJ)/%.o: $(D_SRC)/%.c $(H)
	@mkdir -p $(dir $@)
	@echo "[$<] ---> [$@]"
	@$(CC) $(CFLAGS) -I$(D_H) -c $< -o $@

clean:
	@rm -rf $(D_OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
