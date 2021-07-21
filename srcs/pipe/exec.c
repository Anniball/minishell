/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:10:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/21 15:00:24 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	my_exec(t_cmd pip, char **env, char *infile, char *outfile)
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