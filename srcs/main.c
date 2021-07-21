/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 15:52:54 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/21 15:01:13 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char** argv) {
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	while (1) 
  {
    (void)argc;
    (void)argv;
		char *input = readline("➜ minishell > ");
		parse_line(shell, input);
		add_history(input);
		printf("%s\n", input);
	}

	return 0;
}