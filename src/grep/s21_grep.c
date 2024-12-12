

#include "s21_grep.h"
void process_file(char *filename, int flags, char *patterns[],
                  int pattern_count, int file_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
    return;
  }

  char line[1024];
  int match_count = 0;
  int line_number = 0;
  regex_t regex;
  regmatch_t pmatch[1];
  int match_found = 0;  // Флаг для отслеживания наличия совпадений
  int has_matches = 0;  // Флаг наличия совпадений для -l

  while (fgets(line, sizeof(line), file)) {
    line_number++;
    match_found = 0;

    // Проверяем строку на совпадение с каждым шаблоном
    for (int i = 0; i < pattern_count; ++i) {
      const char *pattern = patterns[i];
      int cflags = REG_EXTENDED;

      if (flags & FLAG_I) {
        cflags |= REG_ICASE;
      }

      if (regcomp(&regex, pattern, cflags) != 0) {
        fprintf(stderr, "grep: failed to compile regex: %s\n", pattern);
        fclose(file);
        return;
      }

      // Проверка на совпадение с регулярным выражением
      if (regexec(&regex, line, 1, pmatch, 0) == 0) {
        match_found = 1;
      }

      regfree(&regex);

      if (match_found) {
        break;
      }
    }

    if (flags & FLAG_V) {
      match_found = !match_found;
    }

    if (match_found) {
      match_count++;
      has_matches = 1;  // Устанавливаем флаг наличия совпадений
    }

    if (flags & FLAG_C) {
      continue;
    }

    if (file_count > 1 && match_found && !(flags & FLAG_L)) {
      printf("%s:", filename);
    }

    if (match_found) {
      if (flags & FLAG_N && !(flags & FLAG_L)) {
        printf("%d:%s", line_number, line);
      } else if (!(flags & FLAG_L)) {
        printf("%s", line);
      }
    }
  }

  fclose(file);

  if (flags & FLAG_C) {
    if (file_count > 1) {
      printf("%s:", filename);
    }
    if (flags & FLAG_L && match_count > 0) match_count = 1;
    printf("%d\n", match_count);
  }

  if ((flags & FLAG_L) && has_matches) {
    printf("%s\n", filename);
  }
}

void process_arguments(int argc, char *argv[], int *flags, char *files[],
                       char *patterns[], int *file_count, int *pattern_count) {
  int opt;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      // Если аргумент начинается с '-', это может быть флаг
      optind = i;
      while ((opt = getopt(argc, argv, "ivclne:")) != -1) {
        switch (opt) {
          case 'i':
            *flags |= FLAG_I;
            break;
          case 'v':
            *flags |= FLAG_V;
            break;
          case 'c':
            *flags |= FLAG_C;
            break;
          case 'l':
            *flags |= FLAG_L;
            break;
          case 'n':
            *flags |= FLAG_N;
            break;
          case 'e':
            patterns[*pattern_count] = optarg;
            (*pattern_count)++;
            *flags |= FLAG_E;
            break;
        }
      }
      i = optind - 1;  // Продолжаем с аргумента после обработанных getopt
    } else {
      if (*pattern_count == 0) {
        patterns[*pattern_count] = argv[i];
        (*pattern_count)++;
      } else {
        files[*file_count] = argv[i];
        (*file_count)++;
      }
    }
  }
}

int main(int args, char *argv[]) {
  int flags = 0;
  char *files[100];
  char *patterns[100];
  int file_count = 0;
  int pattern_count = 0;

  // Обработка аргументов командной строки
  process_arguments(args, argv, &flags, files, patterns, &file_count,
                    &pattern_count);

  if (pattern_count == 0) {
    fprintf(stderr, "grep: no patterns specified\n");
    return EXIT_FAILURE;
  }

  if (file_count == 0) {
    fprintf(stderr, "grep: no files specified\n");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < file_count; ++i) {
    process_file(files[i], flags, patterns, pattern_count, file_count);
  }

  return EXIT_SUCCESS;
}