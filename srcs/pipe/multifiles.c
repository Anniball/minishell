/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multifiles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 10:58:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/13 10:43:09 by ldelmas          ###   ########.fr       */
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

static int	reverse_comp(char *s1, char *s2)
{
	ssize_t	len1;
	ssize_t	len2;

	if (!s2)
		return (-1);
	len2 = my_strlen(s2) - 1;
	len1 = my_strlen(s1) - 1;
	if (!s1 || len1 < len2)
		return (1);
	while (len2 >= 0)
	{
		if (s1[len1] != s2[len2])
			return (1);
		len2--;
		len1--;
	}
	return (0);
}

static int	double_infile(t_lst *infile)
{
	// char	*str;
	// char	*name;
	// char	buf;
	// int		ret;
	int		pip[2];
	char	*str;
	char	*input;
	char	*tmp;

	if (pipe(pip) == -1)
		return (-1);
	str = NULL;
	input = readline(">");
	while (input)
	{
		if (!ft_strcmp(infile->str, input))
			break ;
		tmp = my_concat(input, '\n');
		if (!tmp)
		{
			free(str);
			return (-1);
		}
		str = parse_join(str, tmp);
		if (!str)
			return (-1);
		input = readline(">");
	}
	write(pip[1], str, my_strlen(str));
	free(str);
	close(pip[1]);
	// if (pipe(pip) == -1)
	// 	return (-1);
	// name = infile->str;
	// str = ((void *)0);
	// ret = 1;
	// while (ret > 0)
	// {
	// 	write(STDERR_FILENO, "NEW\n", 4);
	// 	ret = read(STDIN_FILENO, &buf, 1);
	// 	if (ret < 0)
	// 		return (-1);
	// 	if (buf == '\n' && !reverse_comp(str, name))
	// 		break ;
	// 	str = my_concat(str, buf);
	// 	if (!str)
	// 		return (-1);
	// }
	// write(pip[1], str, my_strlen(str));
	// free(str);
	// close(pip[1]);
	return (pip[0]);
}

int	multi_outfiles(t_cmd *cmd, int out)
{
	t_lst	*outfiles;
	int		fd;

	outfiles = cmd->outfiles;
	if (!outfiles)
		return (out);
	while (outfiles->next)
	{
		if (outfiles->flag)
			fd = open(outfiles->str, O_CREAT | O_APPEND, 0666);
		else
			fd = open(outfiles->str, O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
			return (-1);
		close(fd);
		outfiles = outfiles->next;
	}
	if (outfiles->flag)
		fd = open(outfiles->str, O_CREAT | O_APPEND | O_RDWR, 0666);
	else
		fd = open(outfiles->str, O_CREAT | O_TRUNC | O_RDWR, 0666);
	return (fd);
}

int	multi_infiles(t_cmd *cmd, int in)
{
	t_lst	*infiles;
	int		fd;

	infiles = cmd->infiles;
	if (!infiles)
		return (in);
	fd = -1;
	while (infiles)
	{
		if (infiles->flag)
		{
			fd = double_infile(infiles);
			if (fd < 0)
				return (-1);
		}
		else
			fd = open(infiles->str, O_RDONLY);
		if (fd < 0)
		{
			write(STDERR_FILENO, cmd->cmd, my_strlen(cmd->cmd));
			write(STDERR_FILENO, ": ", 2);
			write(STDERR_FILENO, cmd->infiles->str,
				my_strlen(cmd->infiles->str));
			write(STDERR_FILENO, ": No such file or directory\n", 28);
			return (-1);
		}
		else if (infiles->next)
			close(fd);
		infiles = infiles->next;
	}
	return (fd);
}
