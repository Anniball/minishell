#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	is_in_str(char *str, char c)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (1);
	return (0);
}

size_t	my_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*my_strcpy(char *src, char *dst)
{
	size_t	i;

	if (!src || !dst)
		return ((void *)0);
	i = my_strlen(src);
	dst[i--] = '\0';
	while (i + 1)
	{
		dst[i] = src[i];
		i--;
	}
	return (dst);
}

char	*my_strdup(char *str)
{
	char	*new;

	if (!str)
		return ((void *)0);
	new = malloc(sizeof(*new) * (my_strlen(str) + 1));
	if (!new)
		return ((void *)0);
	new = my_strcpy(str, new);
	return (new);
}


static int words_count_with_quotes(char *str, char c)
{
	int i;
	int count;
	char	open_quote;
	char	*new_str;
	char	*env_var;
	int		last_join;

	i = -1;
	new_str = NULL;
	last_join = 0;
	open_quote = 0;
	count = 0;
	if (str == NULL)
		return (0);
	while (str[++i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (open_quote == str[i])
				open_quote = 0;
			else if (open_quote == 0 && is_in_str(&str[i] + 1, str[i]))
				open_quote = str[i];
		}
		if (i > 0 && str[i - 1] != 0 && str[i - 1] != c && str[i] == c && open_quote == 0)
		{
			count++;
			str[i] = 0;
		} else if (i > 0 && str[i - 1] == 0 && str[i - 1] != c && str[i] == c && open_quote == 0) {
			str[i] = 0;
		}
	}
	if (str[i - 1] && str[i - 1] != c)
		count++;
	return (count);
}

char **parse_split_with_quotes(char *str, char c)
{
	const int str_len = my_strlen(str);
	const int w_count = words_count_with_quotes(str, c);
	char **split_list;
	int i;
	int current_word;

	current_word = 0;
	printf("%d\n", w_count);
	i = -1;
	split_list = malloc(sizeof(char *) * (w_count + 1));
	split_list[w_count] = NULL;
	while (++i < str_len)
	{
		if (str[i] == 0)
			;
		else
		{
			split_list[current_word] = my_strdup(&str[i]);
			i += my_strlen(split_list[current_word]) - 1;
			current_word++;
		}
	}
	return (split_list);
}

int main(int argc, char **argv)
{
	char **split;
	if (argc != 2)
		split = parse_split_with_quotes(my_strdup("'hey''hey'"), ' ');
	else
		split = parse_split_with_quotes(argv[1], ' ');
	int i;
	i = -1;
	if (!split)
		return (0);
	while (split[++i])
		printf("%d: %s\n", i, split[i]);
	return (0);
}