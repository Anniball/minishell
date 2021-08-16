/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 17:06:18 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/16 10:16:48 by tpetit           ###   ########.fr       */
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
	int	i;

	i = -1;
	while (str_list[++i])
		free(str_list[i]);
	free(str_list);
}

static int	init_shell_env(t_shell *shell, char **env)
{
	int				i;
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

t_shell	*init_edit_shell(int is_init, char **env, int status)
{
	static t_shell	*shell;

	if (is_init == 2)
		return (shell);
	else if (is_init == 3)
	{
		clear_shell(shell);
		return (NULL);
	}
	else if (is_init)
	{
		shell = malloc(sizeof(t_shell));
		if (!shell)
			return (NULL);
		shell->start_cmd = NULL;
		shell->status = 0;
		shell->cmd_start = 0;
		init_shell_env(shell, env);
		return (shell);
	}
	if (shell)
		shell->status = status;
	return (NULL);
}

static char	*create_shell_line(t_shell *shell, char **env)
{
	const int	status = shell->status;
	char		*line;
	char		*old_line;
	char		*pwd;

	if (status)
		line = my_strdup(RED "➜" BLU " minishell ( ");
	else
		line = my_strdup(GRN "➜" BLU " minishell ( " );
	pwd = get_env_value(shell, env, "PWD");
	old_line = line;
	line = my_strjoin(line, pwd);
	free(old_line);
	free(pwd);
	old_line = line;
	line = my_strjoin(line, " ) > " RESET);
	free(old_line);
	return (line);
}

/*
**	FORBIDDEN FCT rl_clear_history
*/

void	clear_shell(t_shell *shell)
{
	int		i;
	char	**env;

	rl_clear_history();
	env = shell->env;
	i = -1;
	while (env[++i])
		free(env[i]);
	free(env);
	cmd_clear(&shell->start_cmd);
	free(shell);
}

static void	main_loop(t_shell *shell, char *line, char *input)
{
	line = create_shell_line(shell, shell->env);
	input = readline(line);
	add_history(input);
	free(line);
	if (!input)
	{
		write(1, "exit\n", 5);
		init_edit_shell(3, NULL, 0);
		exit(0);
	}
	else if (!*input)
		;
	else if (!check_line(shell, input))
	{
		parse_line(shell, input);
		print_cmd(shell);
		shell->cmd_start = 1;
		n_piper(shell);
	}
	shell->cmd_start = 0;
	free(input);
}

static void	set_shlvl(t_shell *shell)
{
	char *env_var;
	char	*tmp;
	int		sh_lvl;
	int		i;

	env_var = get_env_value(shell, shell->env, "SHLVL");
	if (my_strlen(env_var) > 3 || ft_atoi(env_var) >= 999)
	{
		tmp = ft_itoa(sh_lvl);
		write(STDERR_FILENO, "minishell: warning: shell level (", 33);
		write(STDERR_FILENO, env_var, my_strlen(env_var));
		write(STDERR_FILENO, ") too high, resetting to 1\n", 27);
		sh_lvl = 0;
	}
	sh_lvl = ft_atoi(env_var);
	free(env_var);
	tmp = ft_itoa(sh_lvl + 1);
	env_var = my_strjoin("SHLVL=", tmp);
	free(tmp);
	i = -1;
	while (shell->env[++i])
	{
		if (ft_strcmp(shell->env[i], "SHLVL") == 0)
		{
			free(shell->env[i]);
			shell->env[i] = env_var;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		i;
	char	*input;
	char	*line;

	argc = (int)argc;
	argv = (void *)argv;
	line = NULL;
	input = NULL;
	shell = init_edit_shell(1, envp, 0);
	set_shlvl(shell);
	i = -1;
	receive_signal();
	while (++i < 10)
		main_loop(shell, line, input);
	clear_shell(shell);
	return (0);
}
