/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 11:21:49 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/09 16:00:27 by ldelmas          ###   ########.fr       */
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

static char *get_str(char **env, char *str)
{
	int i;

	i = 0;
	while (env[i] && my_scmp(env[i], str))
		i++;
	return (env[i]);
}


static char **change_pwd(char **env)
{
    char    *tmp;
    char    *tmp_new;
    char    *tmp_old;
    int     i;
    int     j;

    tmp = malloc(sizeof(*tmp) * PATH_MAX);
    if (!tmp)
        return ((void *)0);
    i = 0;
    while (env[i] && my_scmp(env[i], "OLDPWD="))
        i++;
    j = 0;
    while (env[j] && my_scmp(env[j], "PWD="))
        j++;
    tmp_new = my_strjoin("OLD", env[j]);
    tmp_old = my_strjoin("PWD=", getcwd(tmp, PATH_MAX));
    if (!tmp_new || !tmp_old)
    {
        free(tmp_new);
        free(tmp_old);
        free(tmp);
        return (env);
    }
    free(env[j]);
    free(env[i]);
    free(tmp);
    env[j] = tmp_new;
    env[i] = tmp_old;
    return (env);
}


int get_cd(t_cmd *cmd, char ***env)
{
    int ret;

    if (cmd->flags[1] && cmd->flags[2])
    {
        write(STDERR_FILENO, "Too much arguments for this command.\n", 37);
        return (ERROR);
    }
    else
    {
        if (!cmd->flags[1])
            ret = chdir(get_str(*env, "HOME=") + 5);
        else
        {
            if (cmd->flags[1][0] == '~')
                ret = chdir(cmd->flags[1] + 1);
            ret = chdir(cmd->flags[1]);
        }
        *env = change_pwd(*env);
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