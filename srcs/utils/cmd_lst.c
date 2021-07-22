/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 19:16:15 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/22 08:31:00 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_flags(t_cmd *cmd)
{
	int i;

	i = -1;
	while (cmd->flags[++i] != NULL)
		free(cmd->flags[i]);
	free(cmd->flags);
}

void	cmd_clear(t_cmd **lst)
{
	t_cmd	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->cmd);
		//free_flags((*lst));
		free(*lst);
		*lst = tmp;
	}
}

void	cmd_add_back(t_cmd **alst, t_cmd *new)
{
	t_cmd *tmp;

	if (!alst || !*alst)
	{
		if (!alst)
			return ;
		*alst = new;
		return ;
	}
	tmp = *alst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}


t_cmd	*cmd_new(char *cmd, char **flags)
{
	t_cmd	*new;

	if (!(new = malloc(sizeof(t_cmd))))
		return (NULL);
	new->cmd = cmd;
	new->flags = flags;
	new->next = NULL;
	return (new);
}

