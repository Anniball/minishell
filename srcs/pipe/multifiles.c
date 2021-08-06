/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multifiles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 10:58:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/06 17:06:20 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*read_file(int fd)
{
	int 	ret;
	char	buffer[BUFFER_SIZE];
	char	*cat;
	char	*tmp;

	ret = 1;
	cat = ((void *)0);
	while (ret)
	{
		ret = read(fd, buffer, 1);
		if (ret < 0)
		{
			free(cat);
			return ((void *)0);
		}
		tmp = cat;
		cat = my_strjoin(cat, buffer);
		free(tmp);
	}
	return (cat);
}

int	multi_outfiles(t_cmd *cmd, int out)
{
	t_lst	*outfiles;
	int		fd;
	int		tmp;

	outfiles = cmd->outfiles;
	if (!outfiles)
		return (1);
	while (outfiles->next)
	{
		fd = open(outfiles->str, O_CREAT | O_TRUNC);
		if (fd < 0)
			return (-1);
		close(fd);
		outfiles = outfiles->next;
	}
	fd = open(outfiles->str, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (out < 0)
		return (fd);
	if (fd < 0)
	{
		close(out);
		return (-1);
	}
	if (dup2(fd, out))
	{
		close(fd);
		close(out);
		return (-1);
	}
	close(fd);
	return (out);
}

static int	write_in_pipes(t_lst *infiles, int *pip)
{
	int		fd;
	char	*str;

	fd = open(infiles->str, O_RDONLY);
	if (fd < 0)
	{
		close(pip[1]);
		close(pip[0]);
		return (-1);
	}
	str = read_file(fd);
	if (str)
		write(pip[1], str, my_strlen(str));
	free(str);
	close(fd);
	return (pip[0]);
}

int	multi_infiles(t_cmd *cmd)
{
	t_lst	*infiles;
	int		pip[2];
	char	*str;

	infiles = cmd->infiles;
	if (!infiles)
		return (0);
	if (pipe(pip) == -1)
		return (-1);
	else if (!infiles->next)
		return (write_in_pipes(infiles, pip));
	while (infiles)
	{
		if (write_in_pipes(infiles, pip) < 0)
			return (-1);
		infiles = infiles->next;
	}
	close(pip[1]);
	return (pip[0]);
}

int	concat_infiles(t_cmd *cmd, int old_infile)
{
	int		pip[2];
	t_lst	*infiles;
	char	*str;

	if (pipe(pip) == -1)
		return (-1);
	infiles = cmd->infiles;
	if (old_infile >= 0)
	{
		str = read_file(old_infile);
		if (str)
			write(pip[1], str, my_strlen(str));
		free(str);
	}
	close(old_infile);
	if (!infiles)
		close(pip[1]);
	while (infiles->next)
	{
		if (write_in_pipes(infiles, pip) < 0)
			return (-1);
		infiles = infiles->next;
	}
	close(pip[1]);
	return (pip[0]);
}
