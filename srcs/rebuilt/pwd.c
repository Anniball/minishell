/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/22 10:04:21 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/04 11:12:10 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_pwd(t_cmd *cmd)
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
