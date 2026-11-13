#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct options flags;
  int error = 0;

  initialize_flags(&flags);

  if (parse_flags(argc, argv, &flags)) {
    if (optind < argc) {
      for (int i = optind; i < argc; i++) {
        cat_print(argv[i], flags);
      }
    } else {
      fprintf(stderr, "s21_cat: no file provided\n");
      error = 1;
    }
  } else {
    error = 1;
  }

  return error;
}