#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_grep.h"

void initialize_flags(struct options *flags) {
  flags->e = 0;
  flags->i = 0;
  flags->v = 0;
  flags->c = 0;
  flags->l = 0;
  flags->n = 0;
  flags->h = 0;
  flags->s = 0;
  flags->f = 0;
  flags->o = 0;
}

int parse_arguments(int argc, char *argv[], struct options *flags) {
  char ch;
  int error = 0;
  struct patterns pattern;
  pattern.buffer_size = 0;
  pattern.buffer = NULL;
  int pattern_count = 0;

  while ((ch = getopt(argc, argv, "eivclnhsf:o")) != -1 && !error) {
    switch (ch) {
      case 'e':
        flags->e = 1;
        error = add_pattern(optarg, &pattern, flags);
        pattern_count++;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        error = add_pattern_from_file(optarg, &pattern, flags);
        pattern_count++;
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        break;
    }
  }


    if (optind < argc) {
    // Добавляем шаблон из аргументов командной строки
    //pattern.buffer = argv[optind];
    //pattern.buffer_size = strlen(argv[optind]);
    error = add_pattern(argv[optind], &pattern, flags);
    optind++;
    
    // Выполняем grep с собранными шаблонами
    error = do_all(&pattern, *flags, argc, argv);
    
    // НЕ ОСВОБОЖДАЕМ pattern.buffer, если он указывает на argv
    // (мы не выделяли память под него)
  } else {
    // Если нет шаблонов, используем по умолчанию
    pattern.buffer = ".*"; // или другой шаблон по умолчанию
    pattern.buffer_size = 2;
    error = do_all(&pattern, *flags, argc, argv);
  }


  return error;
}

int do_all(struct patterns *pattern, struct options flags, int argc,
           char *argv[]) {
  int error = 0;
  regex_t re;

  if (argc - optind == 1) flags.h = 1;

  if (!error) {
    if (flags.i)
      error = regcomp(&re, pattern->buffer, REG_EXTENDED | REG_ICASE);
    else
      error = regcomp(&re, pattern->buffer, REG_EXTENDED);

    for (int i = optind; i < argc; i++) {
      print_grep(argv[i], flags, &re);
    }
    regfree(&re);
  }

  return error;
}

int add_pattern_from_file(char *filename, struct patterns *pattern, struct options *flags) {
  int error = 0;
  FILE *file = fopen(filename, "r");
  char *line = NULL;
  size_t n = 0;
  int len_line = 0;

  if (file == NULL) {
    if (!flags->s) fprintf(stderr, "s21_grep: no such file or directory\n");
    error = 1;
  } else {
    while ((len_line = getline(&line, &n, file) != -1 && !error)) {
      if (len_line > 0 && line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
      }
      error = add_pattern(line, pattern, flags);
    }
    if (line) free(line);
    if (pattern->buffer) free(pattern->buffer);
  }
  fclose(file);

  return error;
}

int add_pattern(char *string, struct patterns *pattern, struct options *flags) {
  int error = 0;
    int str_len = 0;
    
    if (string) str_len = strlen(string);

    if (pattern->buffer_size == 0) {
      pattern->buffer = malloc((3 + str_len) * sizeof(char));

      if (pattern->buffer == NULL) {
        if (!flags->s) fprintf(stderr, "Failed to allocate memory");
        error = 1;
      } else {
        pattern->buffer_size = str_len + 3;
        pattern->buffer[0] = '\0';
        strcat(pattern->buffer, "(");
        strcat(pattern->buffer, string);
        strcat(pattern->buffer, ")");
      }
    } else if (str_len != 0) {
      pattern->buffer = realloc(pattern->buffer, pattern->buffer_size + str_len + 3);
      if (pattern->buffer == NULL) {
        if (!flags->s) fprintf(stderr, "Failed to allocate memory");
        error = 1;
      } else {
        pattern->buffer_size = pattern->buffer_size + str_len + 3;
        strcat(pattern->buffer, "|(");
        strcat(pattern->buffer, string);
        strcat(pattern->buffer, ")");
      }
    }
    return error;
}

void print_grep(char *filename, struct options flags, regex_t *re) {
  FILE *file = fopen(filename, "r");
  char *line = NULL;
  size_t n = 0;
  int line_count = 0, match_count = 0, result = 0, line_len = 0;

  if (file == NULL) {
    if (!flags.s) fprintf(stderr, "s21_grep: no such file or directory\n");
  } else {
    while ((line_len = getline(&line, &n, file)) != -1) {
      result = !regexec(re, line, 0, NULL, 0);
      line_count++;
      match_count += (result + flags.v) % 2;
      if ((result && !flags.v) || (!result && flags.v)) {
        if (!flags.c && !flags.l) {
          if (flags.o)
            print_match(flags, line, filename, line_count, re);
          else {
            if (!flags.h) printf("%s:", filename);
            if (flags.n) printf("%d:", line_count);
            printf("%s", line);
            if (line[line_len - 1] != '\n') printf("\n");
          }
        }
      }
    }
    if (flags.c && !flags.l) {
      if (!flags.h) printf("%s:", filename);
      printf("%d\n", match_count);
    }
    if (flags.l && match_count > 0) printf("%s\n", filename);
    if (line) free(line);
    fclose(file);
  }
}

void print_match(struct options flags, char *line, char *filename,
                 int line_count, regex_t *re) {
  regmatch_t match;
  int offset = 0;

  if (line[strlen(line) - 1] == '\n' && line[strlen(line) - 1] == EOF) {
    line[strlen(line) - 1] = '\0';
  }

  while (1) {
    if ((long unsigned int)offset > strlen(line)) break;
    int result = regexec(re, line + offset, 1, &match, 0);
    if (result != 0) break;
    if (match.rm_eo != 0) {
      if (!flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", line_count);
      // if (flags.e) {
      //   printf("%s", line);
      //   if (line[strlen(line) - 1] != '\n') printf("\n");
      // } 
      if (flags.o) {
        for (int i = match.rm_so; i < match.rm_eo; i++) {
          putchar(line[offset + i]);
        }
        putchar('\n');
        offset += match.rm_eo;
      }
    } else {
      offset++;
    }
  }
}