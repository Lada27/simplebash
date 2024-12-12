CC = gcc 
FLAGS = -Wall -Werror -Wextra
STAND =  -std=c11

.PHONY: s21_cat all clean rebuild test

s21_cat:
	$(CC) $(FLAGS) $(STAND) s21_cat.c -o s21_cat

all: s21_cat

clean:
	rm -f s21_cat

rebuild: clean all

test: s21_cat
	cd test_cat && sh tests.sh && cd ..
	