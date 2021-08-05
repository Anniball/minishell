/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 11:23:10 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/05 12:08:25 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	words_count(char *str, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (str == NULL)
		return (0);
	while (str[++i])
	{
		if (str[i - 1] != c && str[i] == c)
			count++;
	}
	if (str[i - 1] && str[i - 1] != c)
		count++;
	return (count);
}

char	**parse_split(char *str, char c)
{
	const int	w_count = words_count(str, c);
	const int	str_len = my_strlen(str);
	char		**split_list;
	int			i;
	int			current_word;

	current_word = 0;
	i = -1;
	split_list = malloc(sizeof(char *) * (w_count + 1));
	split_list[w_count] = NULL;
	while (str[++i])
		if (str[i] == c)
			str[i] = 0;
	i = -1;
	while (++i < str_len)
	{
		if (str[i] != 0)
		{
			split_list[current_word] = my_strdup(&str[i]);
			i += my_strlen(split_list[current_word]) - 1;
			current_word++;
		}
	}
	return (split_list);
}

void	set_quote(char *str, int i, char *quote)
{
	if (str[i] == '"' || str[i] == '\'')
	{
		if (*quote == str[i])
			*quote = 0;
		else if (*quote == 0 && is_in_str(&str[i] + 1, str[i]))
			*quote = str[i];
	}
}

static int	words_count_with_quotes(char *str, char c)
{
	int		i;
	int		count;
	char	quote;
	int		last_join;

	i = -1;
	last_join = 0;
	quote = 0;
	count = 0;
	if (str == NULL)
		return (0);
	while (str[++i])
	{
		set_quote(str, i, &quote);
		if (i && str[i - 1] && str[i - 1] != c && str[i] == c && !quote)
		{
			count++;
			str[i] = 0;
		}
		else if (i && !str[i - 1] && str[i - 1] != c && str[i] == c && !quote)
			str[i] = 0;
	}
	if (str[i - 1] && str[i - 1] != c)
		count++;
	return (count);
}

char	**parse_split_with_quotes(char *str, char c)
{
	const int	str_len = my_strlen(str);
	const int	w_count = words_count_with_quotes(str, c);
	char		**split_list;
	int			i;
	int			current_word;

	current_word = 0;
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
