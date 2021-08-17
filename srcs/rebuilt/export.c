/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 10:58:21 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/17 11:47:43 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**strptradd(char **ptr, char *str)
{
	char	**new;
	char	*tmp;
	int		size;

	size = 0;
	while (ptr[size])
		size++;
	new = malloc(sizeof(*new) * (size + 2));
	tmp = malloc(sizeof(*tmp) * (my_strlen(str) + 1));
	if (!new || !tmp)
	{
		free(new);
		free(tmp);
		return ((void *)0);
	}
	size = -1;
	while (ptr[++size])
		new[size] = ptr[size];
	tmp = my_strcpy(str, tmp);
	new[size] = tmp;
	new[size + 1] = (void *)0;
	free(ptr);
	return (new);
}

char	**dispatch_if_yet(char **env, char *str)
{
	char	*s;
	int		i;
	char	*tmp;

	s = strdup_until_c(str, '=');
	if (!s)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if (ft_strcmp(env[i], s) == 0)
		{
			tmp = my_strdup(str);
			if (!tmp)
				return (NULL);
			free(env[i]);
			env[i] = tmp;
			return (env);
		}
	}
	return (strptradd(env, str));
}

int	get_export(t_cmd *cmd, char ***env)
{
	int		i;
	char	**old_env;

	if (!cmd->flags[1])
		return (get_env(*env, cmd));
	else if (cmd->flags[1] && cmd->flags[2])
	{
		write(STDERR_FILENO, "Incorrect arguments for this command.\n", 38);
		return (ERROR);
	}
	i = 0;
	while (cmd->flags[1][i] && cmd->flags[1][i] != '=')
		i++;
	if (!cmd->flags[1][i])
		return (0);
	old_env = *env;
	*env = dispatch_if_yet(*env, cmd->flags[1]);
	if (!(*env))
	{
		*env = old_env;
		return (-1);
	}
	return (0);
}
