/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/21 20:00:07 by tpetit           ###   ########.fr       */
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
	return (split_list);
}

static char *get_cmd_from_line(char *str)
{
	int i;
	char *cmd;
	int last;
	int len;

	i = -1;
	len = 0;
	last = 0;
	cmd = NULL;
	while (str[++i])
	{
		if (str[i] == ' ')
		{
			if (len != 0)
			{
				str[i] = 0;
				break;
			}
			else
				last = i + 1;
		}
		else
			len++;
	}
	return (my_strdup(&str[last]));
}

int	parse_line(t_shell *shell, char *line)
{
	char **split_line;
	int i;

	i = -1;
	split_line = parse_split(line, '|');
	while (split_line[++i] != NULL)
	{
		printf("command %d (cmd len %s): |%s|\n", i, get_cmd_from_line(split_line[i]), split_line[i]);
	}
}