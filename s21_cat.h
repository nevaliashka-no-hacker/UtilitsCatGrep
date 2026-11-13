#ifndef CAT
#define CAT

struct options {
  int b, e, n, s, t, v;
};

void initialize_flags(struct options *flags);
int parse_flags(int argc, char *argv[], struct options *flags);
void cat_print(char filename[], struct options flags);

#endif  // CAT