/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/26 09:31:58 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parent_pipe(int *fds, t_cmd *pip, char **env, char *outfile)
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
	my_command(pip->cmd, pip->flags, env);
	if (outfile)
		close(out);
}

static void	brother_pipe(int *fds, t_cmd *pip, char **env, char *outfile)
{
	int	pid;
	int	new_fds[2];

	wait(0);
	close(fds[1]);
	if (pipe(new_fds) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	else if (!pid)
	{
		close(new_fds[0]);
		if (dup2(fds[0], STDIN_FILENO) < 0)
			return ;
		close(fds[0]);
		if (dup2(new_fds[1], STDOUT_FILENO) < 0)
			return ;
		my_command(pip->cmd, pip->flags, env);
		close(new_fds[1]);
	}
	else if (!pip->next->next)
		parent_pipe(new_fds, pip->next, env, outfile);
	else
		brother_pipe(new_fds, pip->next, env, outfile);
}

static void	child_pipe(int *fds, t_cmd *pip, char **env, char *infile)
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
	my_command(pip->cmd, pip->flags, env);
	if (infile)
		close(in);
}

int	n_piper(t_cmd *pip, char **env, char *infile, char *outfile)
{
	int	pid;
	int	fds[2];

	if (!pip->next)
		my_exec(*pip, env, infile, outfile);
	else if (!pip->next->next)
		piper(*pip, env, infile, outfile);
	else
	{
		if (pipe(fds) == -1)
			return (-1);
		pid = fork();
		if (pid == -1)
			return (-1);
		if (!pid)
			child_pipe(fds, pip, env, infile);
		else
			brother_pipe(fds, pip->next, env, outfile);
	}
	return (0);
}

/*CHECKING MAIN*/
/*	gcc -I "../../includes/" n_pipe.c pipe.c exec.c command.c ../utils/basics.c */

// int main(int ac, char **av, char **env)
// {
// 	t_cmd	cmd1;
// 	t_cmd	cmd2;
// 	t_cmd	cmd3;

// 	cmd1.cmd = "grep";
// 	cmd1.next = &cmd2;
// 	char *flags1[] = {cmd1.cmd, "-a", "q", (void *)0};
// 	cmd1.flags = flags1;

// 	cmd2.cmd = "wc";
// 	cmd2.next = &cmd3;
// 	char *flags2[] = {cmd2.cmd, "-l", (void *)0};
// 	cmd2.flags = flags2;

// 	cmd3.cmd = "cat";
// 	cmd3.next = (void *)0;
// 	char *flags3[] = {cmd3.cmd, "-e", (void *)0};
// 	cmd3.flags = flags3;

// 	n_piper(&cmd1, env, "in", "out");
// }