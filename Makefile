main : main.c tinysquish.c tinysquish.h Makefile
	gcc -o main main.c tinysquish.c -g -O0 -Wall -Werror -Wextra