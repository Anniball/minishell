/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 10:57:19 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/11 10:03:32 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** don't forget to FREE ALL!!
*/

int	get_exit(int status, int main)
{
	init_edit_shell(3, NULL, 0);
	if (status == MALLOC_ERROR)
		write(STDOUT_FILENO, "Minishell: malloc error\n", 24);
	else if (main)
		write(STDOUT_FILENO, "exit\n", 5);
	exit(status);
}
