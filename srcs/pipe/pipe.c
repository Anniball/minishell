/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/02 15:44:55 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parent_pipe(int *fds, t_cmd pip, char **env, char *outfile)
{
	int		out;

	wait(0);
	close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO))
		return ;
	close(fds[0]);
	if (outfile)
	{
		out = open(outfile, O_RDWR | O_CREAT | O_TRUNC);
		if (out == -1)
			return ;
		if (dup2(out, STDOUT_FILENO) < 0)
			return ;
	}
	my_command(pip, pip.cmd, pip.flags, env);
	if (outfile)
		close(out);
}

static void	child_pipe(int *fds, t_cmd pip, char **env, char *infile)
{
	int		in;

	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		return ;
	close(fds[1]);
	if (infile)
	{
		in = open(infile, O_RDONLY);
		if (in == -1)
			return ;
		if (dup2(in, STDIN_FILENO) < 0)
			return ;
	}
	my_command(pip, pip.cmd, pip.flags, env);
	if (infile)
		close(in);
}

int	piper(t_cmd pip, char **env, char *infile, char *outfile)
{
	int	pid;
	int	fds[2];

	if (pipe(fds) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (!pid)
		child_pipe(fds, pip, env, infile);
	else
		parent_pipe(fds, *(pip.next), env, outfile);
	return (0);
}

/*CHECKING MAIN*/
/*	gcc -I "../../includes/" pipe.c exec.c ../utils/basics.c */

// int main(int ac, char **av, char **env)
// {
// 	t_cmd cmd1;
// 	t_cmd cmd2;

// 	cmd1.cmd = "grep";
// 	cmd1.next = &cmd2;
// 	char *flags1[] = {cmd1.cmd, "-a", "q", (void *)0};
// 	cmd1.flags = flags1;

// 	cmd2.cmd = "wc";
// 	cmd2.next = (void *)0;
// 	char *flags2[] = {cmd2.cmd, "-l", (void *)0};
// 	cmd2.flags = flags2; 

// 	piper(cmd1, env, "in", "out");

// }