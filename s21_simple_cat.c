#include <getopt.h>
#include <stdio.h>

#include "s21_cat.h"

void initialize_flags(struct options *flags) {
  flags->b = 0;
  flags->e = 0;
  flags->n = 0;
  flags->s = 0;
  flags->t = 0;
  flags->v = 0;
}

int parse_flags(int argc, char *argv[], struct options *flags) {
  int ch;
  int not_error = 1;

  while ((ch = getopt(argc, argv, "benstETv")) != -1) {
    switch (ch) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default:
        not_error = 0;
        break;
    }
  }

  return not_error;
}

void cat_print(char filename[], struct options flags) {
  char pr = '\n';
  int ch = 0;
  int count = 1, blank_count = 0;
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
    return;
  }

  while ((ch = fgetc(file)) != EOF) {
    if (flags.s) {
      if (pr == '\n' && ch == '\n') {
        blank_count++;
        if (blank_count > 1) continue;
      } else {
        blank_count = 0;
      }
    }

    if (flags.b) {
      if (pr == '\n' && ch != '\n') {
        fprintf(stdout, "%6d\t", count);
        count++;
      }
    } else if (flags.n) {
      if (pr == '\n') {
        fprintf(stdout, "%6d\t", count);
        count++;
      }
    }

    if (flags.t) {
      if (ch == '\t') {
        fputc('^', stdout);
        ch = 'I';
      }
    }

    if (flags.e) {
      if (ch == '\n') {
        fputc('$', stdout);
      }
    }

    if (flags.v) {
      if ((ch >= 0 && ch < 9) || (ch > 10 && ch < 32)) {
        fputc('^', stdout);
        ch += 64;
      } else if (ch == 127) {
        fputc('^', stdout);
        ch = 63;
      }
    }

    fputc((char)ch, stdout);
    pr = ch;
  }

  fclose(file);
}