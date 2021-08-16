/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 09:37:43 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/16 09:58:21 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ministrlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

// static void	my_sub_sigint(int sig)
// {
// 	write(1, "\b\b", 2);
// }

// static void	my_sub_sigquit(int sig)
// {
// 	write(1, "\b\b", 2);
// }

static void	my_sigint(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
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

int main(int argc, char** argv, char **envp)
{
	int i;

	i = -1;
	signal(SIGINT, &my_sigint);
	signal(SIGQUIT, &my_sigquit);
	while (++i < 5) 
	{
		char *input = readline(GRN "➜" BLU " minishell > " RESET);
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		else if (!*input)
			continue;
		int pid = fork();
		if (pid == 0)
		{
			if (execve(input, &input, envp) < 0)
			{
				write(STDOUT_FILENO, "No executable having this name.\n", 33);
				return (0);
			}
		}
		else
		{
			wait(0);
			printf("Program over.\n");
		}
		free(input);
	}
	return 0;
}