/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 10:58:21 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/17 11:11:38 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**strptradd(char **ptr, char *str)
{
	char	**new;
	int		size;

	size = 0;
	while (ptr[size])
		size++;
	new = malloc(sizeof(*new) * (size + 2));
	if (!new)
		return ((void *)0);
	size = -1;
	while (ptr[++size])
	{
		new[size] = my_strdup(ptr[size]);
		if (!new[size])
			return ((void *)0);
		free(ptr[size]);
	}
	new[size] = my_strdup(str);
	new[size + 1] = (void *)0;
	free(ptr);
	return (new);
}

char	**dispatch_if_yet(char **env, char *str)
{
	char	*s;
	int		i;

	s = strdup_until_c(str, '=');
	i = -1;
	while (env[++i])
	{
		if (ft_strcmp(env[i], s) == 0)
		{
			env[i] = my_strdup(str);
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
		get_env(*env, cmd);
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

/*CHECKING MAIN*/
/*gcc export.c env.c ../utils/basics.c ../pipe/command.c*/

// int main(int ac, char **av, char **env)
// {
// 	t_cmd cmd;
// 	char **new_env;
// 	size_t count;

// 	count = 0;
// 	while (env[count])
// 		count++;
// 	new_env = malloc(sizeof(*new_env) * (count + 1));
// 	count = -1;
// 	while (env[++count])
// 		new_env[count] = my_strdup(env[count]);
// 	new_env[count] = (void *)0;

// 	cmd.cmd = "export";
// 	char *flags[] = {cmd.cmd, "TEST=coucou", NULL};
// 	cmd.flags = flags;
// 	cmd.next = (void *)0;
// 	get_export(&cmd, &new_env);
// 	char *exe[] = {"bash", NULL};

// 	system("leaks a.out");
// 	my_command(*exe, exe, new_env);
// }