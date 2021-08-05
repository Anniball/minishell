/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/05 16:19:26 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		set_quote(str, i, &open_quote);
		if (str[i] == '$' && str[i + 1] && !(open_quote == '\''))
		{
			new_str = parse_join(new_str, ft_substr(str,
						last_join, i - last_join));
			env_var = strdup_until(&str[i + 1], " \"'");
			new_str = parse_join(new_str, get_env_value(shell, env, env_var));
			last_join = i + my_strlen(env_var) + 1;
			free(env_var);
		}
	}
	if (last_join != i)
		new_str = parse_join(new_str, ft_substr(str, last_join, i - last_join));
	free(str);
	return (new_str);
}

char	*get_next_word(char *str, int *index)
{
	int		i;
	int		len;
	char	*next_word;

	i = -1;
	len = 0;
	while (str[++i])
	{
		if (is_in_str(" ><", str[i]) && len != 0)
			break ;
		else if (!is_in_str(" ><", str[i]))
			len++;
	}
	*index = *index + i;
	next_word = malloc(sizeof(char) * (len + 1));
	i = -1;
	len = 0;
	while (str[++i])
	{
		if (is_in_str(" ><", str[i]) && len != 0)
			break ;
		else if (!is_in_str(" ><", str[i]))
		{
			next_word[len] = str[i];
			len++;
		}
	}
	next_word[len] = 0;
	return (next_word);
}

char	*get_input_output(t_cmd	*new, char *cmd)
{
	char	*new_cmd;
	int		i;
	int		j;
	char	quote;
	t_lst	*new_file;

	new_cmd = malloc(sizeof(char) * (my_strlen(cmd) + 1));
	quote = 0;
	i = -1;
	j = -1;
	while (cmd[++i])
	{
		set_quote(cmd, i, &quote);
		new_file = lst_new(NULL);
		if (!quote && cmd[i] == '>')
		{
			if (cmd[i + 1] == '>')
				new_file->flag = 1;
			new_file->str = get_next_word(&cmd[i + 1], &i);
			lst_add_back(&new->outfiles, new_file);
		}
		else if (!quote && cmd[i] == '<')
		{
			if (cmd[i + 1] == '<')
				new_file->flag = 1;
			new_file->str = get_next_word(&cmd[i + 1], &i);
			lst_add_back(&new->infiles, new_file);
		}
		else if (++j > -1)
			new_cmd[j] = cmd[i];
	}
	new_cmd[j + 1] = 0;
	return (new_cmd);
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
		new = cmd_new(NULL, NULL);
		strip = get_input_output(new, split_line[i]);
		strip = replace_by_env_value(shell, shell->env, strip);
		strip = my_strip(strip, ' ');
		strip_list = parse_split_with_quotes(strip, ' ');
		remove_close_quote_from_lst(strip_list);
		new->cmd = strip_list[0];
		new->flags = strip_list;
		cmd_add_back(&shell->start_cmd, new);
		free(strip);
		free(split_line[i]);
		split_line[i] = NULL;
	}
	free(split_line);
	return (0);
}
