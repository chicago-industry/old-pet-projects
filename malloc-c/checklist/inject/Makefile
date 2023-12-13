.PHONY: hook

CC=gcc -std=c99 -Wall

hook: hook-main hook-preload.so 
	./hook-main
	LD_PRELOAD=./hook-preload.so ./hook-main

hook-main: hook-main.c
	${CC} -O0 -Wl, -export-dynamic -g -ggdb -o $@ $^ -ldl

hook-preload.so: hook-preload.c
	${CC} -O0 -fPIC -shared -g -ggdb -o $@ $^ -ldl