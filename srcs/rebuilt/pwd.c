/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/22 10:04:21 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/28 14:49:11 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		get_pwd(t_cmd *cmd)
{
	char	cwd[PATH_MAX];

	if (cmd->flags[1])
	{
		write(STDERR_FILENO, "Too much arguments for this command.\n", 68);
		return (ERROR);
	}
	if (getcwd(cwd, PATH_MAX))
	{
		write(STDOUT_FILENO, cwd, my_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (SUCCESS);
	}
	else
		return (ERROR);
}