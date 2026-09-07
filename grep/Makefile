CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra -g

CAT_OBJ = s21_grep.o 


.PHONY: all clean rebuild

all: s21_grep

s21_grep: s21_grep.c s21_simple_grep.c s21_grep.h
	$(CC) $(CFLAGS) -o s21_grep *.c

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm s21_grep

rebuild: clean all
	