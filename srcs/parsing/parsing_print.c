/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 19:15:35 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/21 23:13:11 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_cmd(t_shell *shell)
{
	t_cmd *start;
	int i;
	int j;

	start = shell->start_cmd;
	i = 0;
	while (start)
	{
		j = -1;
		printf("--------------------\n");
		printf("     Command %d\n", ++i);
		printf("Command name: " MAG "%s\n" RESET, start->cmd);
		printf("Flags: " CYN);
		while (start->flags[++j] != NULL)
			printf("%s ", start->flags[j]);
		printf(RESET "\n");
		start = start->next;
	}
	printf("--------------------\n");
}