#ifndef GREP
#define GREP

#include <regex.h>

struct options {
  int e, i, v, c, l, n, h, s, f, o;
};

struct patterns {
  char *buffer;
  int buffer_size;
};

void initialize_flags(struct options *flags);
int parse_arguments(int argc, char *argv[], struct options *flags);
int do_all(struct patterns *pattern, struct options flags, int argc,
           char *argv[]);
int add_pattern_from_file(char *filename, struct patterns *pattern, struct options *flags);
int add_pattern(char *string, struct patterns *pattern, struct options *flags);
void print_grep(char filename[], struct options flags, regex_t *re);
void print_match(struct options flags, char *line, char *filename,
                 int line_count, regex_t *re);

#endif  // GREP
