/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 15:52:54 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/22 10:14:19 by tpetit           ###   ########.fr       */
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
	int i;

	shell = malloc(sizeof(t_shell));
	int f = fork();
	if (f == 0)
	{
		my_command("clear", parse_split("clear", ' '), envp);
		exit(0);
	}
	waitpid(f, 0, 0);
	init_shell(shell);
	get_pwd();
	i = -1;
	while (++i < 5) 
	{
		char *input = readline(GRN "➜" BLU " minishell > " RESET);
		if (!input || !input[0])
			continue;
		parse_line(shell, input);
		add_history(input);
		print_cmd(shell);
	}
	clear_history();
	cmd_clear(&shell->start_cmd);
	free(shell);
	return 0;
}