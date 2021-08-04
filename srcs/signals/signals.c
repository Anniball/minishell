/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:54:53 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/04 15:02:48 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	my_sigint(int sig)
{
	write(1, "\n", 1);
	// write(1, GRN "➜" BLU " minishell >  \n" RESET, 16);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void my_sigquit(int sig)
{
	// rl_replace_line(1, "\b \b\b \b", 6);
	rl_on_new_line();
	rl_redisplay();
}

void receive_signal(void)
{
	signal(SIGINT, &my_sigint);
	signal(SIGQUIT, &my_sigquit);
}