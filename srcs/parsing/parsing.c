/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/11 10:10:43 by ldelmas          ###   ########.fr       */
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
	return (new_str);
}

static int	get_next_word_len(char *str, int *index, int *i_toset, char *quote_toset)
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
	*quote_toset = 0;
	return (len);
}

char	*get_next_word(char *str, int *index)
{
	int		i;
	int		len;
	char	*next_word;
	char	quote;
	char	last_quote;

	len = get_next_word_len(str, index, &i, &last_quote);
	next_word = malloc(sizeof(char) * (len + 1));
	if (!next_word)
		get_exit(MALLOC_ERROR, 1);
	quote = 0;
	len = 0;
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
	if (!new_cmd)
		get_exit(MALLOC_ERROR, 1);
	quote = 0;
	i = -1;
	j = -1;
	while (cmd[++i])
	{
		set_quote(cmd, i, &quote);
		if (!quote && is_in_str("<>", cmd[i]))
		{
			new_file = lst_new(NULL);
			if (!new_file)
				get_exit(MALLOC_ERROR, 1);
			if (cmd[i + 1] == cmd[i])
				new_file->flag = 1;
			if (cmd[i] == '>')
				lst_add_back(&new->outfiles, new_file);
			else
				lst_add_back(&new->infiles, new_file);
			new_file->str = get_next_word(&cmd[i + 1], &i);
		}
		else if (++j > -1)
			new_cmd[j] = cmd[i];
	}
	new_cmd[j + 1] = 0;
	free(cmd);
	return (new_cmd);
}

static	int	init_parse_free(t_parse_free **parse_free)
{
	*parse_free = malloc(sizeof(t_parse_free));
	if (!parse_free)
		get_exit(MALLOC_ERROR, 1);
	(*parse_free)->quote = 0;
	(*parse_free)->split_list = NULL;
	(*parse_free)->strip_list = NULL;
	(*parse_free)->strip = NULL;
	return (0);
}

int	parse_line(t_shell *shell, char *line)
{
	t_parse_free *p;
	int		i;
	t_cmd	*new;

	i = -1;
	init_parse_free(&p);
	cmd_clear(&shell->start_cmd);
	shell->start_cmd = NULL;
	p->split_list = parse_split_with_quotes(line, '|');
	while (p->split_list[++i] != NULL)
	{
		new = cmd_new(NULL, NULL);
		p->strip = replace_by_env_value(shell, shell->env, p->split_list[i]);
		p->strip = get_input_output(new, p->strip);
		p->strip = my_strip(p->strip, ' ');
		p->strip_list = parse_split_with_quotes(p->strip, ' ');
		remove_close_quote_from_lst(p->strip_list);
		new->cmd = p->strip_list[0];
		new->flags = p->strip_list;
		cmd_add_back(&shell->start_cmd, new);
		free(p->strip);
		free(p->split_list[i]);
		p->split_list[i] = NULL;
	}
	free(p->split_list);
	free(p);
	return (0);
}
