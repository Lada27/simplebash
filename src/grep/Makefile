CC = gcc 
FLAGS = -Wall -Werror -Wextra
STAND =  -std=c11

.PHONY: s21_grep all clean rebuild test


s21_grep:
	$(CC) $(FLAGS) $(STAND) s21_grep.c -o s21_grep

all: s21_grep

clean:
	rm -f s21_grep

rebuild: clean all

test: s21_grep
	cd test_grep && sh tests.sh && cd ..
	