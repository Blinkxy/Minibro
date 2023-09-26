/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Final_frees.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 00:12:35 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/26 01:07:32 by mzoheir          ###   ########.fr       */
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
		free(tmp->redir);
		tmp = tmp->next;
	}
}

void	free_struct(t_define *define)
{
	int	i;

	i = -1;
	while (++i < define->size_struct)
		free(define[i].content);
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
        {
			free(tmp->final_cmd[i]);
            i++;
        }
		free(tmp->final_cmd);
		tmp = tmp->next;
	}
}
void	free_cmd(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		while (tmp->cmd[i])
        {
			free(tmp->cmd[i]);
            i++;
        }
		free(tmp->cmd);
		tmp = tmp->next;
	}
}

void	free_all(t_list *cmds)
{
	t_list	*tmp;
    
	free_redir(cmds);
    free_final_cmd(cmds);
    free_cmd(cmds);
    tmp = cmds;
	while (tmp)
	{
		free_struct(tmp->define);
		cmds = tmp->next;
		free(tmp);
		tmp = cmds;
	}
}
