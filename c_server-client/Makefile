BIN1 = src_client
BIN2 = src_server

MOD = -sC
# MOD = -C

all: 
	@$(MAKE) $(MOD) $(BIN1)
	@$(MAKE) $(MOD) $(BIN2)

clean: 
	@$(MAKE) $(MOD) $(BIN1) clean
	@$(MAKE) $(MOD) $(BIN2) clean

fclean:
	@$(MAKE) $(MOD) $(BIN1) fclean
	@$(MAKE) $(MOD) $(BIN2) fclean

re:
	@$(MAKE) $(MOD) $(BIN1) re
	@$(MAKE) $(MOD) $(BIN2) re

debug:
	@$(MAKE) $(MOD) $(BIN1) debug
	@$(MAKE) $(MOD) $(BIN2) debug

format:
	@$(MAKE) $(MOD) $(BIN1) format
	@$(MAKE) $(MOD) $(BIN2) format
