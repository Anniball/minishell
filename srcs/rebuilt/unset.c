/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 10:28:16 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/17 14:58:54 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*charjoin(char *str, char c)
{
	char	*new;
	size_t	len;

	len = my_strlen(str);
	new = malloc(sizeof(*new) * (len + 2));
	if (!new)
	{
		free(str);
		return ((void *)0);
	}
	new = my_strcpy(str, new);
	new[len] = c;
	new[len + 1] = '\0';
	free(str);
	return (new);
}

static char	**trunc_elem(char **tab, int pos)
{
	int		size;
	int		new_pos;
	char	**new;

	size = 0;
	while (tab[size])
		size++;
	new = malloc(sizeof(*new) * size);
	if (!new)
		return ((void *)0);
	size = -1;
	new_pos = 0;
	while (tab[++size])
	{
		if (size != pos)
			new[new_pos++] = tab[size];
		else
			free(tab[size]);
	}
	new[new_pos] = (void *)0;
	free(tab);
	return (new);
}

int	get_unset(t_cmd *cmd, char ***env)
{
	int		i;
	char	**old_env;

	if (!cmd->flags[1] || (cmd->flags[1] && cmd->flags[2]))
	{
		write(STDERR_FILENO, "Incorrect arguments for this command.\n", 38);
		return (ERROR);
	}
	old_env = *env;
	cmd->flags[1] = charjoin(cmd->flags[1], '=');
	if (!cmd->flags[1])
		return (ERROR);
	i = 0;
	while ((*env)[i] && my_scmp((*env)[i], cmd->flags[1]))
		i++;
	if (!(*env)[i])
		return (0);
	*env = trunc_elem(*env, i);
	if (!(*env))
	{
		*env = old_env;
		return (ERROR);
	}
	return (0);
}
