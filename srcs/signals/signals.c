/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:54:53 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/13 16:43:42 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	my_sigint(int sig)
{
	if (init_edit_shell(2, NULL, 0)->cmd_start)
	{
		write(STDOUT_FILENO, "\n", 1);
		return ;
	}
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** rl_replace_line(1, "\b \b\b \b", 6);
*/

static void	my_sigquit(int sig)
{
	if (init_edit_shell(2, NULL, 0)->cmd_start)
	{
		write(STDOUT_FILENO, "Quit: 3\n", 8);
		return ;
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	receive_signal(void)
{
	signal(SIGINT, &my_sigint);
	signal(SIGQUIT, &my_sigquit);
}
