#ifndef SPLIT_QUOTES_HPP
# define SPLIT_QUOTES_HPP

#include <stdlib.h>

char **split_quotes(char const *s, char c);
void free_arr(char **arr);

#endif
