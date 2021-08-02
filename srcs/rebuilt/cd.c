/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 11:21:49 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/30 12:08:00 by ldelmas          ###   ########.fr       */
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

static char *get_home(char **env)
{
	int i;

	i = 0;
	while (env[i] && my_scmp(env[i], "HOME="))
		i++;
	return (env[i] + 5);
}

int get_cd(t_cmd *cmd, char **env)
{
	int ret;

	if (cmd->flags[1] && cmd->flags[2])
	{
		write(STDERR_FILENO, "Too much arguments for this command.\n", 37);
		return (ERROR);
	}
	else if (!cmd->flags[1])
		ret = chdir(get_home(env));
	else
	{
		if (cmd->flags[1][0] == '~')
			ret = chdir(cmd->flags[1] + 1);
		ret = chdir(cmd->flags[1]);
	}
	if (ret)
		write(1, "Directory not found.\n", 21);
	return (ret);
}

/*CHECKING MAIN*/

// int main(int ac, char **av, char **env)
// {
// 	t_cmd cmd;

// 	cmd.cmd = "cd";
// 	char *flags[] = {cmd.cmd, "/Users/ldelmas/Desktop", NULL};
// 	cmd.flags = flags;
// 	cmd.next = NULL;
// 	get_cd(&cmd, env);
// 	char cwd[PATH_MAX];
// 	printf("%s\n", getcwd(cwd, PATH_MAX));
// }