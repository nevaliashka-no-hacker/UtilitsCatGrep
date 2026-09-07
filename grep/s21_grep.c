#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  struct options flags;
  int error;

  initialize_flags(&flags);
  error = parse_arguments(argc, argv, &flags);

  return error;
}