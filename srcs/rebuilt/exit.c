/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 10:57:19 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/02 11:00:22 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_exit(t_cmd *cmd, char **env)
{
	//FREE ALL
	write(STDOUT_FILENO, "exit\n", 5);
	exit(EXIT_SUCCESS);
}