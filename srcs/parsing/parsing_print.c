/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 19:15:35 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/04 11:21:12 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_cmd(t_shell *shell)
{
	t_cmd	*start;
	int		i;
	int		j;

	start = shell->start_cmd;
	i = 0;
	while (start)
	{
		j = -1;
		printf("--------------------\n     Command %d\n", ++i);
		printf("Command name: " MAG "%s\n" RESET, start->cmd);
		printf("Flags: [ " CYN);
		while (start->flags[++j] != NULL)
		{
			if (j == 0)
				printf(MAG "%s" CYN, start->flags[j]);
			else
				printf("%s", start->flags[j]);
			if (start->flags[j + 1] != NULL)
				printf(RESET ", " CYN);
		}
		printf(RESET " ]\n");
		start = start->next;
	}
	printf("--------------------\n");
}
