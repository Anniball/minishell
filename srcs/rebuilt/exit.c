/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 10:57:19 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/10 18:04:11 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** don't forget to FREE ALL!!
*/

int	get_exit(int status)
{
	init_edit_shell(3, NULL, 0);
	if (status == MALLOC_ERROR)
		write(STDOUT_FILENO, "Minishell: malloc error\n", 24);
	else
		write(STDOUT_FILENO, "exit\n", 5);
	exit(status);
}
