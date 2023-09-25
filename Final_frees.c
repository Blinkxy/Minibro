/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Final_frees.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 00:12:35 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/26 00:13:03 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	free_redir(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	tmp = cmds;
	while (tmp)
	{
		i = -1;
		while (++i < tmp->red_nb)
		{
			free(tmp->redir[i].delimiter);
			free(tmp->redir[i].file);
			free(tmp->redir[i].red);
		}
		if (tmp->redir)
			free(tmp->redir);
		tmp = tmp->next;
	}
}

void	free_struct(t_define *define)
{
	int	i;

	i = 0;
	while (i < define->size_struct)
	{
		if (define[i].content)
			free(define[i].content);
		i++;
	}
	if (define)
		free(define);
}

void	free_final_cmd(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		while (tmp->final_cmd[i])
			free(tmp->final_cmd[i]);
		if (tmp->final_cmd)
			free(tmp->final_cmd);
		tmp = tmp->next;
	}
}

void	free_all(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	tmp = cmds;
	while (tmp)
	{
		free_struct(tmp->define);
		i = -1;
		if (cmds->cmd && cmds->cmd[i])
		{
			while (++i < tmp->size_cmd)
				free(cmds->cmd[i]);
			free(cmds->cmd);
		}
		free_redir(cmds);
		free_final_cmd(cmds);
		cmds = tmp->next;
		free(tmp);
		tmp = cmds;
	}
	free(cmds);
}
