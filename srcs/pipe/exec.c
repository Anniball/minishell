/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/09 17:14:28 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	my_exec(t_cmd pip, char ***env, int infile, int outfile)
{
	int	ret;
	int	std[2];

	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	if (std[0] < 0 || std[1] < 0)
		return (-1);
	if (infile != STDIN_FILENO)
		if (infile == -1 || dup2(infile, STDIN_FILENO) < 0)
			return (-1);
	if (outfile != STDOUT_FILENO)
		if (outfile == -1 || dup2(outfile, STDOUT_FILENO) < 0)
			return (-1);
	my_command(pip, pip.cmd, pip.flags, env);
	if (infile != STDIN_FILENO)
	{
		close (infile);
		if (dup2(std[0], STDIN_FILENO) < 0)
			return (-1);
		close(std[0]);
	}
	if (outfile != STDOUT_FILENO)
	{
		close (outfile);
		if (dup2(std[1], STDOUT_FILENO) < 0)
			return (-1);
		close(std[1]);
	}
	return (0);
}

// int	my_exec(t_cmd pip, char ***env, int infile, int outfile)
// {
// 	int	ret;
// 	int	std[2];

// 	if (infile != STDIN_FILENO)
// 		if (infile == -1 || dup2(STDIN_FILENO, std[0]) < 0 || dup2(infile, STDIN_FILENO) < 0)
// 			return (-1);
// 	if (outfile != STDOUT_FILENO)
// 		if (outfile == -1 || dup2(STDOUT_FILENO, std[1]) < 0 || dup2(outfile, STDOUT_FILENO) < 0)
// 			return (-1);
// 	my_command(pip, pip.cmd, pip.flags, env);
// 	if (infile != STDIN_FILENO && dup2(std[0], STDIN_FILENO) < 0)
// 	{
// 		close (infile);
// 		close(std[0]);
// 	}
// 	if (outfile != STDOUT_FILENO && dup2(std[1], STDOUT_FILENO) < 0)
// 	{
// 		close (outfile);
// 		close(std[1]);
// 	}
// 	return (0);
// }

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