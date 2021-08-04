/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/04 10:21:07 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_pid_set_value(t_shell *shell, int pid)
{
	int status;

	waitpid(pid, &status, WUNTRACED | WCONTINUED);
	if (shell)
		shell->status = status;
}

static void	parent_pipe(int *fds, t_cmd *pip, char ***env, char *outfile)
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
	my_command(*pip, pip->cmd, pip->flags, env);
	if (outfile)
		close(out);
	exit(EXIT_SUCCESS);
}

static void	brother_pipe(int *fds, t_cmd *pip, char ***env, char *outfile)
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
		my_command(*pip, pip->cmd, pip->flags, env);
		close(new_fds[1]);
	}
	else if (!pip->next->next)
		parent_pipe(new_fds, pip->next, env, outfile);
	else
		brother_pipe(new_fds, pip->next, env, outfile);
	exit(EXIT_SUCCESS);
}

static void	child_pipe(int *fds, t_cmd *pip, char ***env, char *infile)
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
	my_command(*pip, pip->cmd, pip->flags, env);
	if (infile)
		close(in);
	exit(EXIT_SUCCESS);
}

static int	my_builtins(t_cmd *pip, char ***env, char *infile, char *outfile)
{
	int	ret;

	if (!check_builtins(pip->cmd) || pip->next)
		return (1);
	return (my_exec(*pip, env, infile, outfile));
}

int	n_piper(t_shell *shell, char *infile, char *outfile)
{
	int	pid;
	int	fds[2];

	pid = my_builtins(shell->start_cmd, &shell->env, infile, outfile);
	if (pid != 1)
		return (pid);
	pid = fork();
	if (!pid)
	{
		if (!shell->start_cmd->next)
			my_exec(*shell->start_cmd, &shell->env, infile, outfile);
		else if (!shell->start_cmd->next->next)
			piper(*shell->start_cmd, &shell->env, infile, outfile);
		else
		{
			if (pipe(fds) == -1)
				return (-1);
			pid = fork();
			if (pid == -1)
				return (-1);
			if (!pid)
				child_pipe(fds, shell->start_cmd, &shell->env, infile);
			else
				brother_pipe(fds, shell->start_cmd->next, &shell->env, outfile);
		}
		exit(EXIT_SUCCESS);
	}
	wait_pid_set_value(shell, pid);
	return (0);
}

/* CHECKING MAIN */
/*	gcc -I "../../includes/" n_pipe.c pipe.c exec.c builtins.c command.c ../utils/basics.c ../rebuilt/cd.c ../rebuilt/echo.c ../rebuilt/env.c ../rebuilt/exit.c ../rebuilt/export.c ../rebuilt/pwd.c ../rebuilt/unset.c*/

// int main(int ac, char **av, char **env)
// {
// 	t_cmd	cmd1;
// 	t_cmd	cmd2;
// 	t_cmd	cmd3;

// 	cmd1.cmd = "cat";
// 	cmd1.next = &cmd2;
// 	char *flags1[] = {cmd1.cmd, "in", (void *)0};
// 	cmd1.flags = flags1;

// 	cmd2.cmd = "env";
// 	cmd2.next = &cmd3;
// 	char *flags2[] = {cmd2.cmd, (void *)0};
// 	cmd2.flags = flags2;

// 	cmd3.cmd = "cat";
// 	cmd3.next = (void *)0;
// 	char *flags3[] = {cmd3.cmd, "-e", (void *)0};
// 	cmd3.flags = flags3;

// 	n_piper(&cmd1, env, "in", "out");
// }
