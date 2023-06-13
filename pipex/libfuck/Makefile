# NAME OF LIB
NAME = libfuck.a

# SETTINGS
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# HEADER
D_H = include
H = libfuck.h
H := $(H:%.h=$(D_H)/%.h)

# SRC
D_SRC = src
SRC = \
	ft_memdel.c \
	ft_memdup.c \
	ft_memset.c \
	ft_memcpy.c \
	ft_strjoin.c \
	ft_strsplit.c \
	ft_strlen.c \
	ft_strsub.c \
	ft_strdup.c \
	ft_strstr.c \
	ft_strnstr.c \
	ft_strchr.c \
	ft_strcmp.c \
	ft_putstr_fd.c \
	ft_getline.c \
	ft_free_str_arr.c
SRC := $(SRC:%.c=$(D_SRC)/%.c)

# OBJ
D_OBJ = .obj
OBJ := $(SRC:$(D_SRC)/%.c=$(D_OBJ)/%.o)

# RULES
all: $(NAME)

$(NAME): $(OBJ)
	@ar -rc $(NAME) $(OBJ)
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
