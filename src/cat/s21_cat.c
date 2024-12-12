#include "s21_cat.h"

// Функция для добавления знака '$' в конец строки
void append_dollar_sign(char *line) {
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = '\0';
    strcat(line, "$\n");
  }
}

// Функция для обработки табуляций как ^I
void handle_tabs(char *line) {
  char temp[1024];
  int i = 0, j = 0;

  while (line[i] != '\0') {
    if (line[i] == '\t') {
      temp[j++] = '^';
      temp[j++] = 'I';
    } else {
      temp[j++] = line[i];
    }
    i++;
  }
  temp[j] = '\0';
  strcpy(line, temp);
}

// Функция для обработки невидимых символов
void handle_visible_chars(char *line) {
  char temp[1024];
  int i = 0, j = 0;

  while (line[i] != '\0') {
    if ((line[i] >= 32 && line[i] <= 126) || line[i] == '\t' ||
        line[i] == '\n') {
      temp[j++] = line[i];
    } else {
      temp[j++] = '^';
      temp[j++] = line[i] + '@';
    }
    i++;
  }
  temp[j] = '\0';
  strcpy(line, temp);
}

// Функция для обработки файла
void process_file(FILE *file, int flags) {
  char line[1024];
  int line_number = 1;
  int last_line_blank = 0;

  while (fgets(line, sizeof(line), file)) {
    int is_blank = (line[0] == '\n');

    if (flags & FLAG_S && is_blank && last_line_blank) {
      continue;
    }

    if (flags & FLAG_B && !is_blank) {
      printf("%6d\t", line_number++);
    } else if (flags & FLAG_N) {
      printf("%6d\t", line_number++);
    } else if (is_blank) {
      printf("\t");
    }

    if (flags & FLAG_T) {
      handle_tabs(line);
    }

    if (flags & FLAG_V) {
      handle_visible_chars(line);
    }

    if (flags & FLAG_E) {
      append_dollar_sign(line);
    }

    fputs(line, stdout);

    last_line_blank = is_blank;
  }
}

void process_arguments(int args, char *argv[], int *flags, char *files[],
                       int *file_count) {
  int opt;
  static struct option long_options[] = {
      {"number", no_argument, NULL, 'n'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"verbose", no_argument, NULL, 'v'},
      {"tabs", no_argument, NULL, 'T'},
      {"end-of-line", no_argument, NULL, 'E'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(args, argv, "+benstvTE", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        *flags |= FLAG_B;
        break;
      case 'e':
        *flags |= FLAG_E | FLAG_V;  // Флаг -e включает флаг -v
        break;
      case 'n':
        *flags |= FLAG_N;
        break;
      case 's':
        *flags |= FLAG_S;
        break;
      case 't':
        *flags |= FLAG_T | FLAG_V;  // Флаг -t включает флаг -v
        break;
      case 'v':
        *flags |= FLAG_V;
        break;
      case 'T':
        *flags |= FLAG_T;
        break;
      case 'E':
        *flags |= FLAG_E;
        break;
    }
  }

  for (int i = optind; i < args; ++i) {
    files[*file_count] = argv[i];
    (*file_count)++;
  }
}

int main(int args, char *argv[]) {
  int flags = 0;
  char *files[100];
  int file_count = 0;

  process_arguments(args, argv, &flags, files, &file_count);

  if (file_count == 0) {
    process_file(stdin, flags);
  } else {
    // Обработка каждого файла
    for (int i = 0; i < file_count; ++i) {
      FILE *file = fopen(files[i], "r");
      if (!file) {
        fprintf(stderr, "cat: %s: No such file\n", files[i]);
        continue;
      }
      process_file(file, flags);
      fclose(file);
    }
  }

  return EXIT_SUCCESS;
}