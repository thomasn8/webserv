#include "split_quotes.hpp"

static char	*word_dup(const char *str, int word_n, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	if (start == end && !word_n)
		return (NULL);
	word = (char *)malloc((end - start + 1) * sizeof(char));
	while (start < end)
		word[i++] = str[start++];
	word[i] = 0;
	return (word);
}

static int	quotes(char a, char *quote)
{
	if (*quote == '0' && (a == '\'' || a == '\"'))
		*quote = a;
	else if (*quote != '0' && a == *quote)
		*quote = '0';
	return (1);
}

static int	words_count(const char *str, char c, int *i_first_split)
{
	int		i=0, limiter=0, split=0;
	char	quote='0';

	while (*str)
	{
		quotes(*str, &quote);
		if (*str != c && limiter == 0)
		{
			limiter = 1;
			i++;
		}
		else if (*str == c && quote == '0')
		{
			split++;
			limiter = 0;
		}
		str++;
		if (split == 0)
			(*i_first_split)++;
	}
	if (split == 0)
		*i_first_split = -1;
	return (i);
}

static char	**words_split(char **tab, char const *s, char c)
{
	int		i;
	int		start;
	int		end;
	int		word;
	char	quote;

	i = 0;
	start = 0;
	end = 0;
	word = 0;
	quote = '0';
	while (s[i])
	{
		while (s[i] == c && quotes(s[i], &quote) && quote == '0')
			i++;
		start = i;
		while (s[i] && ((s[i] != c && quotes(s[i], &quote)) || quote != '0'))
			i++;
		end = i;
		if (!s[i] && start == end && word > 0)
			return (tab);
		tab[word] = word_dup(s, word, start, end);
		word++;
	}
	return (tab);
}

int split_quotes(char ***tab, char const *s, char c)
{
	size_t	words;
	int i_first_split = 0;

	if (!s)
		return (-1);
	words = words_count(s, c, &i_first_split);
	*tab = (char **)malloc((words + 1) * sizeof(char *));
	if (!*tab)
		return (-1);
	if (!(words_split(*tab, s, c)))
	{
		free(*tab);
		return (-1);
	}
	(*tab)[words] = NULL;
	return (i_first_split);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
