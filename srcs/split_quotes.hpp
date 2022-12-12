#ifndef SPLIT_QUOTES_HPP
# define SPLIT_QUOTES_HPP

#include <stdlib.h>

int split_quotes(char ***tab, char const *s, char c);
void free_arr(char **arr);

#endif
