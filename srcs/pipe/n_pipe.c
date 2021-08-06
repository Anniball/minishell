/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/06 17:00:54 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_pid_set_value(t_shell *shell, int pid)
{
	int status;

	waitpid(pid, &status, WUNTRACED | WCONTINUED);
	if (shell)
		shell->status = status / 256;
}

static void	parent_pipe(int *fds, t_cmd *pip, char ***env)
{
	int		infile;
	int		outfile;

	wait(0);
	close(fds[1]);
	infile = concat_infiles(pip, fds[0]);
	outfile = multi_outfiles(pip, -1);
	if (outfile < 0 || infile < 0 || dup2(infile, STDIN_FILENO))
		return ;
	close(infile);
	if (outfile)
	{
		if (outfile == -1)
			return ;
		if (dup2(outfile, STDOUT_FILENO) < 0)
			return ;
	}
	my_command(*pip, pip->cmd, pip->flags, env);
	if (outfile)
		close(outfile);
	exit(EXIT_SUCCESS);
}

static void	brother_pipe(int *fds, t_cmd *pip, char ***env)
{
	int	pid;
	int	new_fds[2];
	int	infile;
	int	outfile;

	wait(0);
	close(fds[1]);
	if (pipe(new_fds) == -1)
		return ;
	infile = concat_infiles(pip, fds[0]);
	outfile = multi_outfiles(pip, fds[1]);
	if (infile < 0 || outfile < 0)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	else if (!pid)
	{
		if (dup2(infile, STDIN_FILENO) < 0)
			return ;
		close(infile);
		if (dup2(outfile, STDOUT_FILENO) < 0)
			return ;
		close(outfile);
		my_command(*pip, pip->cmd, pip->flags, env);
	}
	else if (!pip->next->next)
		parent_pipe(new_fds, pip->next, env);
	else
		brother_pipe(new_fds, pip->next, env);
	exit(EXIT_SUCCESS);
}

static void	child_pipe(int *fds, t_cmd *pip, char ***env, int infile)
{
	int		in;

	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		return ;
	close(fds[1]);
	if (infile)
		if (infile == -1 || dup2(infile, STDIN_FILENO) < 0)
			return ;
	my_command(*pip, pip->cmd, pip->flags, env);
	if (infile)
		close(infile);
	exit(EXIT_SUCCESS);
}

static int	my_builtins(t_cmd *pip, char ***env)
{
	int	infile;
	int	outfile;

	if (!check_builtins(pip->cmd) || pip->next)
		return (1);
	if (!pip->infiles)
		infile = 0;
	else
		infile = multi_infiles(pip);
	if (pip->outfiles)
		outfile = multi_outfiles(pip, -1);
	else
		outfile = 0;
	if (infile < 0 || outfile < 0)
		return (-1);
	return (my_exec(*pip, env, infile, outfile));
}

int	n_piper(t_shell *shell)
{
	int		pid;
	int		fds[2];
	t_cmd	*cmd;
	int		infile;
	int		outfile;

	cmd = shell->start_cmd;
	if (!cmd)
		return (-1);
	pid = my_builtins(cmd, &shell->env);
	if (pid != 1)
		return (pid);
	pid = fork();
	if (!pid)
	{
		if (!cmd->next)
		{
			infile = multi_infiles(cmd);
			outfile = multi_outfiles(cmd, -1);
			my_exec(*cmd, &shell->env, infile, outfile);
		}
		else
		{
			infile = multi_infiles(cmd);
			outfile = multi_outfiles(cmd, fds[1]);
			if (pipe(fds) == -1)
				return (-1);
			if (infile < 0 || outfile < 0)
				return (-1);
			pid = fork();
			if (pid == -1)
				return (-1);
			if (!pid)
				child_pipe(fds, cmd, &shell->env, infile);
			else
				brother_pipe(fds, cmd->next, &shell->env);
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
