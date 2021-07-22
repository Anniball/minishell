/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 15:52:54 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/22 08:26:44 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_shell(t_shell *shell)
{
	shell->start_cmd = NULL;
}

int main(int argc, char** argv, char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	init_shell(shell);
	while (1) 
	{
		char *input = readline(GRN "➜" BLU " minishell > " RESET);
		parse_line(shell, input);
		add_history(input);
		print_cmd(shell);
	}

	return 0;
}