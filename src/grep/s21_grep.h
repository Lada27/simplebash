#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FLAG_V (1 << 0)
#define FLAG_I (1 << 1)
#define FLAG_E (1 << 2)
#define FLAG_N (1 << 3)
#define FLAG_C (1 << 4)
#define FLAG_L (1 << 5)

void process_file(char *filename, int flags, char *patterns[],
                  int pattern_count, int file_count);
void process_arguments(int argc, char *argv[], int *flags, char *files[],
                       char *patterns[], int *file_count, int *pattern_count);