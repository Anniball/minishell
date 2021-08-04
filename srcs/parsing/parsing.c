/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/04 12:02:47 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** my_strip remove char c from start and end of str.
** This function does not free str.
*/

char	*my_strip(char *str, char c)
{
	int			i;
	int			start;
	int			end;
	const int	len = my_strlen(str);
	char		*ret;

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

int	ft_strcmp(const char *s1, const char *s2)
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

char	*get_env_value(t_shell *shell, char **env, char *var)
{
	int	i;

	i = -1;
	if (var[0] == '?')
		return (ft_itoa(shell->status));
	while (env[++i])
	{
		if (my_strlen(var) < my_strlen(env[i]) && ft_strcmp(env[i], var) == 0
			&& env[i][my_strlen(var)] == '=')
			return (my_strdup(&env[i][my_strlen(var) + 1]));
	}
	return (my_strdup(""));
}

char	*replace_by_env_value(t_shell *shell, char **env, char *str)
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
		if (str[i] == '$' && str[i + 1] && !(open_quote == '\''
				&& is_in_str(&str[i], '\'')))
		{
			new_str = parse_join(new_str, ft_substr(str,
						last_join, i - last_join));
			env_var = strdup_until(&str[i + 1], " \"'");
			new_str = parse_join(new_str, get_env_value(shell, env, env_var));
			last_join = i + my_strlen(env_var) + 1;
		}
	}
	if (last_join != i)
		new_str = parse_join(new_str, ft_substr(str, last_join, i - last_join));
	return (new_str);
}

char	*remove_close_quote(char *str)
{
	char	open_quote;
	int		i;
	int		j;
	char	*new_str;
	int		last_open;

	i = -1;
	j = 0;
	open_quote = 0;
	last_open = -1;
	new_str = malloc(sizeof(char) * (my_strlen(str) + 1));
	while (str[++i])
	{
		if ((str[i] == '"' || str[i] == '\'') && open_quote == 0
			&& is_in_str(&str[i + 1], str[i]))
			open_quote = str[i];
		else if ((str[i] == '"' || str[i] == '\'') && open_quote == str[i])
			open_quote = 0;
		else
		{
			new_str[j] = str[i];
			j++;
		}
	}
	new_str[j] = 0;
	free(str);
	return (new_str);
}

void	remove_close_quote_from_lst(char **lst)
{
	int	i;

	i = -1;
	while (lst[++i])
		lst[i] = remove_close_quote(lst[i]);
}

int	parse_line(t_shell *shell, char *line)
{
	char	**split_line;
	char	*strip;
	char	**strip_list;
	int		i;
	t_cmd	*new;

	i = -1;
	cmd_clear(&shell->start_cmd);
	shell->start_cmd = NULL;
	split_line = parse_split_with_quotes(line, '|');
	while (split_line[++i] != NULL)
	{
		strip = replace_by_env_value(shell, shell->env, split_line[i]);
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
