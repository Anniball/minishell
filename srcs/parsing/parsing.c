/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/02 18:26:40 by tpetit           ###   ########.fr       */
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

char **parse_split(char *str, char c)
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

/*
** get_cmd_from_line get first word of str
*/

static char *get_cmd_from_line(char *str)
{
	int i;
	int last;
	int len;
	char *ret;

	i = -1;
	len = 0;
	last = 0;
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
	ret = my_strdup(&str[last]);
	str[i] = ' ';
	return (ret);
}

/*
** my_strip remove char c from start and end of str. This function does not free str.
*/

char *my_strip(char *str, char c)
{
	int i;
	int start;
	int end;
	const int len = my_strlen(str);
	char *ret;

	i = -1;
	start = 0;
	end = len;
	while (str[++i] && str[i] == c)
		start = i + 1;
	i = -1;
	while (++i < len && str[len - 1 - i] == c)
		end = len - 1 - i;
	str[end] = 0;
	ret = my_strdup(&str[start]);
	return (ret);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	while (1)
	{
		if (!*s1 || !*s2)
			return (0);
		if (*s1 == *s2)
		{
			s1++;
			s2++;
		}
		else
			return ((int)((unsigned char)(*s1) - (unsigned char)(*s2)));
	}
}


char *get_env_value(char **env, char *var)
{
	int i;

	i = -1;
	while (env[++i])
	{
		if (my_strlen(var) < my_strlen(env[i]) && ft_strcmp(env[i], var) == 0 && env[i][my_strlen(var)] == '=')
			return my_strdup(&env[i][my_strlen(var) + 1]);
	}
	return my_strdup("");
}

int	is_in_str(char *str, char c)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (1);
	return (0);
}

char	*strdup_until(const char *str, char *c_lst)
{
	int		i;
	char	*dest;

	i = -1;
	while (str[++i] && !is_in_str(c_lst, str[i]))
		;
	dest = malloc((i + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = -1;
	while (str[++i] && !is_in_str(c_lst, str[i]))
		dest[i] = str[i];
	dest[i] = '\0';
	return (dest);
}

char	*strdup_until_c(const char *str, char c)
{
	int		i;
	char	*dest;

	i = -1;
	while (str[++i] && str[i] != c)
		;
	dest = malloc((i + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = -1;
	while (str[++i] && str[i] != c)
		dest[i] = str[i];
	dest[i] = '\0';
	return (dest);
}

char	*parse_join(char *s1, char *s2)
{
	int		len;
	int		i;
	char	*conc_str;

	len = my_strlen(s1) + my_strlen(s2);
	if (!(conc_str = malloc(sizeof(char) * (len + 1))))
	{
		free(s1);
		return (NULL);
	}
	conc_str[len] = 0;
	i = -1;
	len = my_strlen(s1);
	if (len > 0)
		while (++i < len)
			conc_str[i] = s1[i];
	i = -1;
	if (my_strlen(s2) > 0)
		while (s2[++i])
			conc_str[i + len] = s2[i];
	free(s1);
	return (conc_str);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	int		i;
	size_t	str_len;
	char	*str;

	if (!s)
		return (0);
	if (start > my_strlen(s))
	{
		if (!(str = malloc(sizeof(char))))
			return (NULL);
		str[0] = 0;
	}
	else
	{
		i = -1;
		str_len = len < my_strlen(s) - start ? len : my_strlen(s) - start;
		if (!(str = malloc(sizeof(char) * (str_len + 1))))
			return (NULL);
		while (s[++i] && i < (int)len)
			str[i] = s[start + i];
		str[str_len] = 0;
	}
	return (str);
}

char *replace_by_env_value(char **env, char *str)
{
	char	open_quote;
	int		i;
	char	*new_str;
	char	*env_var;
	int		last_join;

	i = -1;
	new_str = NULL;
	last_join = 0;
	open_quote = 0;
	while (str[++i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (open_quote == str[i])
				open_quote = 0;
			else if (open_quote == 0 && is_in_str(&str[i] + 1, str[i]))
				open_quote = str[i];
		}
		if (str[i] == '$' && str[i + 1] && !(open_quote == '\'' && is_in_str(&str[i], '\'')))
		{
			new_str = parse_join(new_str, ft_substr(str, last_join, i - last_join));
			env_var = strdup_until(&str[i + 1], " \"'");
			new_str = parse_join(new_str, get_env_value(env, env_var));
			last_join = i + my_strlen(env_var) + 1;
		}
	}
	if (last_join != i)
		new_str = parse_join(new_str, ft_substr(str, last_join, i - last_join));
	return new_str;
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

char *remove_close_quote(char *str)
{
	char	open_quote;
	int		i;
	char	*tmp;
	char	*new_str;
	char	*env_var;
	int		last_join;
	int		ok;

	i = -1;
	new_str = NULL;
	open_quote = 0;
	while (str[++i])
	{
		ok = 1;
		if (str[i] == '"' || str[i] == '\'')
		{
			ok = 0;
			if (open_quote == str[i])
				open_quote = 0;
			else if (open_quote == 0 && is_in_str(&str[i + 1], str[i]))
				open_quote = str[i];
			else
				ok = 1;
		}
		if (i > 0 && str[i - 1] == open_quote && ok)
		{
			tmp = strdup_until_c(&str[i], open_quote);
			new_str = parse_join(new_str, tmp);
			i += my_strlen(tmp) - 1;
		}
	}
	free(str);
	return new_str;
}

void	remove_close_quote_from_lst(char **lst)
{
	int i;

	i = -1;
	while (lst[++i])
		lst[i] = remove_close_quote(lst[i]);
}

int	parse_line(t_shell *shell, char *line)
{
	char **split_line;
	char *strip;
	char **strip_list;
	int i;
	t_cmd *new;

	i = -1;
	cmd_clear(&shell->start_cmd);
	shell->start_cmd = NULL;
	split_line = parse_split(line, '|');
	while (split_line[++i] != NULL)
	{
		strip = replace_by_env_value(shell->env, split_line[i]);
		strip = my_strip(strip, ' ');
		strip_list = parse_split_with_quotes(strip, ' ');
		remove_close_quote_from_lst(strip_list);
		new = cmd_new(my_strdup(strip_list[0]), strip_list);
		cmd_add_back(&shell->start_cmd, new);
		free(strip);
		free(split_line[i]);
	}
	free(split_line);
	return (0);
}