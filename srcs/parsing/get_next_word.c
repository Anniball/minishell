/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 12:08:27 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/11 17:00:34 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_next_word_len(char *str, int *index, int *i_toset)
{
	int		i;
	int		len;
	char	quote;
	char	last_quote;

	i = -1;
	len = 0;
	last_quote = 0;
	while (str[++i])
	{
		set_quote(str, i, &quote);
		if (str[i] == quote || str[i] == last_quote)
			;
		else if (is_in_str(" ><", str[i]) && len != 0 && !quote)
			break ;
		else if (!is_in_str(" ><", str[i]) || quote)
			len++;
		last_quote = quote;
	}
	*index = *index + i;
	*i_toset = -1;
	return (len);
}

void	get_next_word_loop(char *str, char quote, int len, char *next_word)
{
	int		i;
	char	last_quote;

	i = -1;
	last_quote = 0;
	while (str[++i])
	{
		set_quote(str, i, &quote);
		if (str[i] == quote || str[i] == last_quote)
			;
		else if (is_in_str(" ><", str[i]) && len != 0 && !quote)
			break ;
		else if (!is_in_str(" ><", str[i]) || quote)
		{
			next_word[len] = str[i];
			len++;
		}
		last_quote = quote;
	}
}

char	*get_next_word(char *str, int *index)
{
	int		i;
	int		len;
	char	*next_word;
	char	quote;

	len = get_next_word_len(str, index, &i);
	next_word = malloc(sizeof(char) * (len + 1));
	if (!next_word)
		return (NULL);
	quote = 0;
	len = 0;
	get_next_word_loop(str, quote, len, next_word);
	next_word[len] = 0;
	return (next_word);
}
