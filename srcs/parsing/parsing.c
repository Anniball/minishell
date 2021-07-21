/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/21 19:14:53 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int words_count(char *str, char c)
{
	int i;
	int count;

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

static char **parse_split(char *str, char c)
{
	const int w_count = words_count(str, c);
	const int str_len = my_strlen(str);
	char **split_list;
	int i;
	int current_word;

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
		if (str[i] == 0)
			;
		else
		{
			split_list[current_word] = my_strdup(&str[i]);
			i += my_strlen(split_list[current_word]) - 1;
			current_word++;
		}
	}
}

int	parse_line(t_shell *shell, char *line)
{
	char **split_line;

	split_line = parse_split(line, '|');
}