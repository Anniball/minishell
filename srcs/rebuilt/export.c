/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 10:58:21 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/27 11:59:53 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**strptradd(char **ptr, char *str)
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
	}
	new[size] = my_strdup(str);
	new[size + 1] = (void *)0;
	//SHOULD I FREE THE OLD ENV ? IS IT MALLOC IN FIRST PLACE ?
	return (new);
}

int	get_export(t_cmd *cmd, char ***env)
{
	int i;

	if (!cmd->flags[1] || (cmd->flags[1] && cmd->flags[2]))
	{
		write(STDERR_FILENO, "Incorrect arguments for this command.\n", 38);
		return (ERROR);
	}
	i = 0;
	while (cmd->flags[1][i] && cmd->flags[1][i] != '=') //we need a better parsing for export
		i++;
	if (!cmd->flags[1][i])
		return (0);
	*env = strptradd(*env, cmd->flags[1]);
	return (0);
}

/*CHECKING MAIN*/
/*gcc export.c ../utils/basics.c ../pipe/command.c*/

// int main(int ac, char **av, char **env)
// {
// 	t_cmd cmd;

// 	cmd.cmd = "export";
// 	char *flags[] = {cmd.cmd, "TEST=coucou", NULL};
// 	cmd.flags = flags;
// 	cmd.next = (void *)0;
// 	get_export(&cmd, &env);
// 	char *exe[] = {"bash", NULL};
// 	my_command(*exe, exe, env);
// }