/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 09:45:11 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/02 15:51:44 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int simple_strcmp(char *s1, char *s2)
{
	int i;
	
	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != 0 && s2[i] != 0)
		i++;
	return (s1[i] - s2[i]);
}

int get_echo(t_cmd *cmd)
{
	int		pos;
	char	end;

	pos = 1;
	end = '\n';
	if (!simple_strcmp(cmd->flags[1], "-n"))
	{
		pos++;
		end = '\0';
	}
	while (cmd->flags[pos])
	{
		write(STDOUT_FILENO, cmd->flags[pos], my_strlen(cmd->flags[pos]));
		pos++;	
	}
	write(STDOUT_FILENO, &end, 1);
	return (0);
}