BIN = ../c_server

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -MMD -MP
LIBS = -lpthread

D_H = ../include
D_SRC = .
D_OBJ = ../obj

H = server.h threadpool.h
H := $(addprefix $(D_H)/, $(H))
SRC = $(wildcard $(D_SRC)/*.c)
OBJ := $(patsubst $(D_SRC)/%.c, $(D_OBJ)/%.o, $(SRC))

all: $(BIN)

$(D_OBJ):
	@mkdir -p $(D_OBJ)

$(D_OBJ)/%.o: $(D_SRC)/%.c | $(D_OBJ)
	@echo "$< => $@"
	@$(CC) $(CFLAGS) -I$(D_H) -c $< -o $@ $(LIBS)

$(BIN): $(OBJ) $(H)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	@echo $@ compiled

debug: CFLAGS += -g -DDEF_DEBUG
debug: clean all

clean:
	@rm -rf $(D_OBJ)

fclean: clean
	@rm -rf $(BIN)

re: fclean all

format:
	@clang-format -i $(D_SRC)/*.c $(D_H)/*.h

-include $(OBJ:.o=.d)

.PHONY: format clean fclean re
