/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multifiles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 10:58:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/11 09:33:41 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		fd = open(infiles->str, O_RDONLY);
		if (fd < 0)
		{
			write(STDERR_FILENO, cmd->cmd, my_strlen(cmd->cmd));
			write(STDERR_FILENO, ": ", 2);
			write(STDERR_FILENO, cmd->infiles->str, my_strlen(cmd->infiles->str));
			write(STDERR_FILENO, ": No such file or directory\n", 28);
			return (-1);
		}
		else if (infiles->next)
			close(fd);
		infiles = infiles->next;
	}
	return (fd);
}
