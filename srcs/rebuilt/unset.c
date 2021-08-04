/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 10:28:16 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/04 11:11:50 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	my_scmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (s1[i] && s2[i])
		return (s1[i] - s2[i]);
	return (s1[i - 1] - s2[i - 1]);
}

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
	while (my_scmp((*env)[i], cmd->flags[1]))
		i++;
	*env = trunc_elem(*env, i);
	if (!(*env))
	{
		*env = old_env;
		return (ERROR);
	}
	return (0);
}

/*CHECKING MAIN*/
/*gcc export.c unset.c ../utils/basics.c ../pipe/command.c*/

// int main(int ac, char **av, char **env)
// {
// 	t_cmd cmd;
// 	size_t count;
// 	char **new_env;

// 	count = 0;
// 	while (env[count])
// 		count++;
// 	new_env = malloc(sizeof(*new_env) * (count + 1));
// 	count = -1;
// 	while (env[++count])
// 		new_env[count] = my_strdup(env[count]);
// 	new_env[count] = (void *)0;

// 	cmd.cmd = "export";
// 	char *flags[] = {cmd.cmd, NULL, NULL};
// 	cmd.flags = flags;
// 	cmd.flags[1] = malloc(sizeof(char) * 200);

// 	cmd.flags[1] = my_strcpy("TEST0=coucou", cmd.flags[1]);
// 	cmd.next = (void *)0;
// 	get_export(&cmd, &new_env);
// 	printf("%s\n", flags[1]);

// 	cmd.flags[1] = my_strcpy("TEST1=hello", cmd.flags[1]);
// 	cmd.next = (void *)0;
// 	get_export(&cmd, &new_env);
// 	printf("%s\n", flags[1]);

// 	cmd.flags[1] = my_strcpy("TEST2=bonjour", cmd.flags[1]);
// 	cmd.next = (void *)0;
// 	get_export(&cmd, &new_env);
// 	printf("%s\n", flags[1]);

// 	cmd.cmd = "unset";

// 	cmd.flags[1] = my_strcpy("TEST1", cmd.flags[1]);
// 	get_unset(&cmd, &new_env);
// 	printf("%s\n", flags[1]);

// 	cmd.flags[1] = my_strcpy("TEST0", cmd.flags[1]);
// 	get_unset(&cmd, &new_env);
// 	printf("%s\n", flags[1]);

// 	free(flags[1]);
// 	system("leaks a.out");
// 	char *exe[] = {"bash", NULL};
// 	my_command(*exe, exe, new_env);
// }
