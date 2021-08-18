/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_infile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 20:30:22 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/18 12:05:17 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*my_concat(char *str, char c)
{
	int		i;
	char	*new;
	size_t	len;

	i = -1;
	len = my_strlen(str);
	new = malloc(sizeof(*new) * (len + 2));
	if (str)
		new = my_strcpy(str, new);
	new[len] = c;
	new[len + 1] = '\0';
	free(str);
	return (new);
}

static char	*eof_error(char *str, char *name)
{
	free(str);
	write(STDERR_FILENO, "minishell: warning: here-document delimited by", 46);
	write(STDERR_FILENO, " end-of-file (wanted '", 22);
	write(STDERR_FILENO, name, my_strlen(name));
	write(STDERR_FILENO, "')\n", 3);
	return (my_strdup(""));
}

static char	*double_in_loop(char *input, t_lst *infile)
{
	char	*tmp;
	char	*str;

	str = (void *)0;
	while (1)
	{
		if (!input)
			return (eof_error(str, infile->str));
		if (!input[0])
			input = (void *)0;
		if (input && !ft_strcmp(infile->str, input))
			break ;
		tmp = my_concat(input, '\n');
		if (!tmp)
		{
			free(str);
			return ((void *)0);
		}
		str = parse_join(str, tmp);
		if (!str)
			return ((void *)0);
		input = readline(">");
	}
	return (str);
}

static void	prompt_sigint(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	double_infile(t_lst *infile)
{
	int		pip[2];
	char	*input;
	char	*str;

	if (pipe(pip) == -1)
		return (-1);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &prompt_sigint);
	input = readline(">");
	if (input && !ft_strcmp(infile->str, input))
		str = my_strdup("");
	else
		str = double_in_loop(input, infile);
	receive_signal();
	if (!str)
	{
		close(pip[1]);
		return (-1);
	}
	write(pip[1], str, my_strlen(str));
	close(pip[1]);
	free(str);
	return (pip[0]);
}
