#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FLAG_B 1
#define FLAG_E 2
#define FLAG_N 4
#define FLAG_S 8
#define FLAG_T 16
#define FLAG_V 32

void append_dollar_sign(char *line);
void handle_tabs(char *line);
void handle_visible_chars(char *line);
void process_file(FILE *file, int flags);
void process_arguments(int args, char *argv[], int *flags, char *files[],
                       int *file_count);
