/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/10 16:03:27 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parent_pipe(int *fds, t_cmd pip, char ***env, char *outfile)
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
	exit(EXIT_SUCCESS);
}

static void	child_pipe(int *fds, t_cmd pip, char ***env, char *infile)
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
	exit(EXIT_SUCCESS);
}

int	piper(t_cmd pip, char ***env, char *infile, char *outfile)
{
	int	pid;
	int	fds[2];
	int	infile;
	int	outfile;

	if (pipe(fds) == -1)
		return (-1);
	infile = concat_infiles(pip, fds[0]);
	outfile = multi_outfiles(pip, new_fds[1]);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (!pid)
		child_pipe(fds, pip, env, infile);
	else
		parent_pipe(fds, *(pip.next), env, outfile);
	exit(EXIT_SUCCESS);
	return (0);
}
