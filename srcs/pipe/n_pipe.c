/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/10 09:11:10 by ldelmas          ###   ########.fr       */
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
	int		files[2];

	wait(0);
	close(fds[1]);
	files[0] = multi_infiles(pip, fds[0]);
	files[1] = multi_outfiles(pip, STDOUT_FILENO);
	if (files[0] < 0 || files[1] < 0 || dup2(files[0], STDIN_FILENO) < 0)
		return ;
	if (files[0] != fds[0])
		close(files[0]);
	close(fds[0]);
	if (files[1] != STDOUT_FILENO && dup2(files[1], STDOUT_FILENO) < 0)
			return ;
	if (files[1] != STDOUT_FILENO)
		close(files[1]);
	my_command(*pip, pip->cmd, pip->flags, env);
	exit(EXIT_SUCCESS);
}

static void	brother_pipe(int *fds, t_cmd *pip, char ***env)
{
	int	pid;
	int	new_fds[2];
	int	files[2];

	wait(0);
	close(fds[1]);
	if (pipe(new_fds) == -1)
		return ;
	files[0] = multi_infiles(pip, fds[0]); //by default the infile is the previous pipe
	files[1] = multi_outfiles(pip, new_fds[1]); //by default the outfile is the pipe
	if (files[0] < 0 || files[1] < 0)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	else if (!pid)
	{
		close(new_fds[0]);
		if (dup2(files[0], STDIN_FILENO) < 0 || dup2(files[1], STDOUT_FILENO) < 0)
			return ;
		if (fds[0] != files[0])
			close(files[0]);
		close(fds[0]);
		if (files[1] != new_fds[1])
			close(files[1]);
		close(new_fds[1]);
		my_command(*pip, pip->cmd, pip->flags, env);
	}
	else if (!pip->next->next)
		parent_pipe(new_fds, pip->next, env);
	else
		brother_pipe(new_fds, pip->next, env);
	exit(EXIT_SUCCESS);
}

static void	child_pipe(int *fds, t_cmd *pip, char ***env, int *files)
{
	int		in;

	close(fds[0]);
	if (files[1] == STDOUT_FILENO)
	{
		if (dup2(fds[1], STDOUT_FILENO) < 0)
			return ;
	}
	else if (dup2(files[1], STDOUT_FILENO) < 0)
			return ;
	close(fds[1]);
	if (files[1 != STDOUT_FILENO])
		close(files[1]);
	if (files[0] != STDIN_FILENO)
	{
		if (dup2(files[0], STDIN_FILENO) < 0)
			return ;
		close(files[0]);
	}
	my_command(*pip, pip->cmd, pip->flags, env);
	exit(EXIT_SUCCESS);
}

static int	my_builtins(t_cmd *pip, char ***env, int infile, int outfile)
{
	int ret;

	if (!check_builtins(pip->cmd) || pip->next)
		return (1);
	if (infile < 0 || outfile < 0)
		return (-1);
	ret = my_exec(*pip, env, infile, outfile);
	return (ret);
}

int	n_piper(t_shell *shell)
{
	int		pid;
	int		fds[2];
	t_cmd	*cmd;
	int		files[2];

	cmd = shell->start_cmd;
	if (!cmd)
		return (-1);
	files[0] = multi_infiles(cmd, STDIN_FILENO);
	files[1] = multi_outfiles(cmd, STDOUT_FILENO);
	if (files[0] < 0 || files[1] < 0)
		return (-1);
	pid = my_builtins(cmd, &shell->env, files[0], files[1]);
	if (pid != 1)
		return (pid);
	pid = fork();
	if (!pid)
	{
		if (!cmd->next)
			my_exec(*cmd, &shell->env, files[0], files[1]);
		else
		{
			if (pipe(fds) == -1)
				return (-1);
			pid = fork();
			if (pid == -1)
				return (-1);
			if (!pid)
				child_pipe(fds, cmd, &shell->env, files);
			else if (cmd->next->next)
				brother_pipe(fds, cmd->next, &shell->env);
			else
				parent_pipe(fds, cmd->next, &shell->env);
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
