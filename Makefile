HEADERS := $(wildcard ./*.h)
SOURCES := $(wildcard ./*.c)
PROJECT := tinysquish

$(PROJECT) : $(HEADERS) $(SOURCES) Makefile
	gcc -o $(PROJECT) $(SOURCES) -lm -g -Wall -Werror -Wextra -O0 -flto