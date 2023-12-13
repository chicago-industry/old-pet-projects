.PHONY: clean fclean re

ifeq ($(HOSTTYPE = ), )
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
NAME_BASE = libft_malloc.so
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g

#-------------------------------------------------------------------------------
D_H = include/
H = malloc.h malloc_error.h
H := $(addprefix $(D_H), $(H))

#-------------------------------------------------------------------------------
D_SRC = src/
SRC = malloc.c page.c main_page.c block.c free.c debug.c extra.c
SRC := $(addprefix $(D_SRC), $(SRC))

#-------------------------------------------------------------------------------
D_OBJ = obj/
OBJ := $(patsubst $(D_SRC)%.c, $(D_OBJ)%.o, $(SRC))

#-------------------------------------------------------------------------------

all: $(NAME)

$(NAME): $(D_OBJ) $(OBJ) Makefile
	@$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ)
	@echo "--------------------------------------"
	@echo "$(NAME) compiled"
	ln -sf $(NAME) $(NAME_BASE)
	@echo "--------------------------------------"

$(D_OBJ):
	@mkdir -p $(D_OBJ)

$(D_OBJ)%.o: $(D_SRC)%.c $(H)
	@echo $<
	@$(CC) $(CFLAGS) -g -I$(D_H) -fPIC -c $< -o $@

clean:
	@rm -rf $(D_OBJ)

fclean: clean
	@rm -rf $(NAME) $(NAME_BASE)

re: fclean all
