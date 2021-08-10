/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 10:58:21 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/10 14:38:04 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static char *my_calloc(size_t size)
// {
// 	char	*new;
// 	size_t	count;

// 	new = malloc(sizeof(*new) * size);
// 	count = -1;
// 	while (++count < size)
// 		new[count] = '\0';
// 	return (new);
// }

// static char *parsing_arg(char *str)
// {
// 	int		i;
// 	int		pos;
// 	char	*new;
// 	char	quote;

// 	new = my_calloc(my_strlen(str) + 1);
// 	if (!new)
// 		return ((void *)0);
// 	i = 0;
// 	pos = 0;
// 	quote = '\0';
// 	while (str[i] && str[i] != '=')
// 		new[pos++] = str[i++];
// 	new[pos++] = str[i];
// 	while (str[++i])
// 	{
// 		if (str[i] == '\\')
// 		{
// 			if (str[i + 1] == '\'' || str[i + 1] == '\"')
// 			{
// 				new[pos++] = str[i + 1];
// 				if (!str[++i])
// 					break ;
// 			}
// 		}
// 		else if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			if (quote && quote != str[i])
// 				new[pos++] = str[i];
// 			else
// 			{
// 				if (quote == str[i])
// 					quote = '\0';
// 				else if (!quote)
// 					quote = str[i];
// 			}
// 		}
// 		else
// 			new[pos++] = str[i];
// 	}
// 	new[pos] = '\0';
// 	if (quote)
// 	{
// 		free(new);
// 		write(STDERR_FILENO, "Wrong number of quotes.\n", 24);
// 		return ((void *)0);
// 	}
// 	printf("%s\n", new);
// 	return (new);
// }

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

static char	**dispatch_if_yet(char **env, char *str)
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