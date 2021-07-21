/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/21 14:04:02 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int		my_scmp(char *s1, char *s2)
{
	int i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (s1[i] && s2[i])
		return (s1[i] - s2[i]);
	return (s1[i - 1] - s2[i - 1]);
}

static void	free_tab(char **tab)
{
	int i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

static char	**my_simple_split(char *str, char c)
{
	int		count;
	int		i;
	int		j;
	char	**tab;

	if (!str)
		return ((void *)0);
	i = 0;
	count = 0;
	while (str[i])
		if (str[i++] == c)
			count++;
	tab = malloc(sizeof(*tab) * (count + 2));
	if (!tab)
		return ((void *)0);
	tab[count + 1] = (void *)0;
	i = 0;
	count = 0;
	while (i < my_strlen(str))
	{
		j = 0;
		while (str[i + j])
			if (str[i + j++] == ':')
				break ;
		if (!str[i + j])
			j++;
		tab[count] = malloc(sizeof(**tab) * j + 1);
		if (!tab[count])
			free_tab(tab);
		tab[count] = my_strncpy(str + i, tab[count], j);
		tab[count][j - 1] = '/';
		tab[count][j] = '\0';
		count++;
		i += j;
	}
	return (tab);
}

void my_command(char *cmd, char **argv, char **env)
{
	char	*full_cmd;
	char	**paths;
	int		i;
	int		ret;

	i = 0;
	while (my_scmp(env[i], "PATH="))
		i++;
	paths = my_simple_split(env[i] + 5, ':');
	i = -1;
	while (paths[++i])
		printf("%s\n", paths[i]);
	ret = execve(*paths, argv, env);
	i = 0;
	while (ret < 0 && paths[i++])
	{
		full_cmd = my_strjoin(paths[i], cmd);
		ret = execve(full_cmd, argv, env);
	}
	free(full_cmd);
	free_tab(paths);
}
int my_exec(t_cmd pip, char **env, char *infile, char *outfile)
{
	int	in;
	int	out;

	if (infile)
	{
		in = open(infile, O_RDONLY);
		if (in == -1 || dup2(in, STDIN_FILENO) < 0)
			return (-1);
	}
	if (outfile)
	{
		out = open(outfile, O_RDWR | O_CREAT | O_TRUNC);
		if (out == -1 || dup2(out, STDOUT_FILENO) < 0)
			return (-1);
	}
	my_command(pip.cmd, pip.flags, env);
	if (infile)
		close (in);
	if (outfile)
		close (out);
	return (0);
}

/*CHECKING MAIN*/
/*	gcc -I "../../includes/" exec.c ../utils/basics.c */

// int main(int ac, char **av, char **env)
// {
// 	t_cmd cmd1;

// 	cmd1.cmd = "grep";
// 	cmd1.next = (void *)0;
// 	char *flags1[] = {cmd1.cmd, "-a", "q", (void *)0};
// 	cmd1.flags = flags1;

// 	my_exec(cmd1, env, "in", "out");
	
// }