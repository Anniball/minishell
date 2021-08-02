/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 15:52:54 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/02 18:00:41 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	str_list_len(char **str_list)
{
	size_t	i;

	i = -1;
	while (str_list[++i])
		;
	return (i);
}

void	free_str_list(char **str_list)
{
	int i;

	i = -1;
	while (str_list[++i])
		free(str_list[i]);
	free(str_list);
}

static int	init_shell_env(t_shell *shell, char **env)
{
	int i;
	const size_t	env_len = str_list_len(env);
	char			**env_copy;

	i = -1;
	env_copy = malloc(sizeof(char *) * (env_len + 1));
	if (env_copy == NULL)
		return (ERROR);
	env_copy[env_len] = NULL;
	while (env[++i])
	{
		env_copy[i] = my_strdup(env[i]);
		if (env_copy[i] == NULL)
		{
			free_str_list(env_copy);
			return (ERROR);
		}
	}
	i = -1;
	shell->env = env_copy;
	return (SUCCESS);
}

static void	init_shell(t_shell *shell, char **env)
{
	shell->start_cmd = NULL;
	init_shell_env(shell, env);
}

int main(int argc, char** argv, char **envp)
{
	t_shell *shell;
	int i;

	shell = malloc(sizeof(t_shell));
	init_shell(shell, envp);
	i = -1;
	while (++i < 5)
	{
		char *input = readline(GRN "➜" BLU " minishell > " RESET);
		if (!input)
		{
			write(1, "Ending minishell.\n", 18);
			break ;
		}
		else if (!*input)
			continue;
		parse_line(shell, input);
		add_history(input);
		n_piper(shell->start_cmd, shell->env, NULL, NULL);
		print_cmd(shell);
	}
	clear_history();
	cmd_clear(&shell->start_cmd);
	free(shell);
	return 0;
}