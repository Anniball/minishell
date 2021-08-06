/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/06 17:06:19 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	my_exec(t_cmd pip, char ***env, int infile, int outfile)
{
	int	ret;

	if (infile)
		if (infile == -1 || dup2(infile, STDIN_FILENO) < 0)
			return (-1);
	if (outfile)
		if (outfile == -1 || dup2(outfile, STDOUT_FILENO) < 0)
			return (-1);
	my_command(pip, pip.cmd, pip.flags, env);
	if (infile)
		close (infile);
	if (outfile)
		close (outfile);
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