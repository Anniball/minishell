/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/16 18:44:29 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_pid_set_value(t_shell *shell, int pid)
{
	int	status;

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
	my_command(pip, pip->cmd, pip->flags, env);
	get_exit(EXIT_SUCCESS, pip);
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
	files[0] = multi_infiles(pip, fds[0]);
	files[1] = multi_outfiles(pip, new_fds[1]);
	if (files[0] < 0 || files[1] < 0)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	else if (!pid)
	{
		close(new_fds[0]);
		if (dup2(files[0], STDIN_FILENO) < 0
			|| dup2(files[1], STDOUT_FILENO) < 0)
			return ;
		if (fds[0] != files[0])
			close(files[0]);
		close(fds[0]);
		if (files[1] != new_fds[1])
			close(files[1]);
		close(new_fds[1]);
		my_command(pip, pip->cmd, pip->flags, env);
	}
	else if (!pip->next->next)
		parent_pipe(new_fds, pip->next, env);
	else
		brother_pipe(new_fds, pip->next, env);
	get_exit(EXIT_SUCCESS, pip);
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
	my_command(pip, pip->cmd, pip->flags, env);
	get_exit(EXIT_SUCCESS, pip);
}

static int	my_builtins(t_cmd *pip, char ***env, int infile, int outfile)
{
	int	ret;

	if (!check_builtins(pip->cmd) || pip->next)
		return (1);
	if (infile < 0 || outfile < 0)
		return (-1);
	ret = my_exec(pip, env, infile, outfile);
	return (ret);
}

int	n_piper(t_shell *shell)
{
	int		pid;
	int		fds[2];
	t_cmd	*cmd;
	int		files[2];

	cmd = shell->start_cmd;
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
			my_exec(cmd, &shell->env, files[0], files[1]);
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
		get_exit(EXIT_SUCCESS, cmd);
	}
	wait_pid_set_value(shell, pid);
	return (0);
}
