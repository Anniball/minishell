/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 09:37:43 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/26 09:38:48 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	my_sub_sigint(int sig)
{
	write(1, "\b\b", 2);
}

static void	my_sub_sigquit(int sig)
{
	write(1, "\b\b", 2);
}

static void	my_sigint(int sig)
{
	write(1, "\b\b\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void my_sigquit(int sig)
{
	write(1, "\b\b", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int main(int argc, char** argv, char **envp)
{
	int i;

	i = -1;
	while (++i < 5) 
	{
		signal(SIGINT, &my_sigint);
		signal(SIGQUIT, &my_sigquit);
		char *input = readline(GRN "➜" BLU " minishell > " RESET);

		if (!input)
		{
			write(1, "Ending minishell.\n", 18);
			break ;
		}
		else if (!*input)
			continue;
		int pid = fork();
		if (pid == 0)
		{
			if (execve(input, &input, envp) < 0)
			{
				write(1, "No executable having this name.\n", 33);
				return (0);
			}
		}
		else
		{
			signal(SIGINT, &my_sub_sigint);
			signal(SIGQUIT, &my_sub_sigquit);
			wait(0);
			printf("Program over.\n");
		}
		free(input);
	}
	return 0;
}