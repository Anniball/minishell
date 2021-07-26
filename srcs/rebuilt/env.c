/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 10:39:18 by ldelmas           #+#    #+#             */
/*   Updated: 2021/07/26 11:06:16 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	 get_env(char **env)
{
	int i;

	if (cmd->flags[1])
	{
		write(STDERR_FILENO, "Too much arguments for this command.\n", 68);
		return (ERROR);
	}
	i = -1;
	while (env[++i])
	{
		write(STDOUT_FILENO, env[i], my_strlen(env[i]));
		write(1, "\n", 1);
	}
	return (0);
}