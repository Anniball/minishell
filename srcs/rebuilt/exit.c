/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 10:57:19 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/11 11:28:45 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_exit(int status, t_cmd *cmd)
{
	int do_print;

	do_print = 0;
	printf("CHECK 1\n");
	if (!cmd || (cmd == init_edit_shell(2, NULL, 0)->start_cmd && !cmd->next))
		do_print = 1;
	printf("CHECK 2\n");
	init_edit_shell(3, NULL, 0);
	if (status == MALLOC_ERROR)
		write(STDERR_FILENO, "Minishell: malloc error\n", 24);
	if (do_print)
		write(STDERR_FILENO, "exit\n", 5);
	exit(status);
}
