/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:48:17 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/25 23:20:54 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	final_cmd(t_list *cmds)
{
	t_index	index;
	t_list	*tmp;

	initialize_index(&index);
	tmp = cmds;
	while (tmp)
	{
		tmp->final_cmd = (char **)malloc(sizeof(char *) * (tmp->size_cmd
					- (tmp->red_nb * 2) + 1));
		index.i = -1;
		while (++index.i < tmp->size_cmd)
		{
			if (tmp->define[index.i].state == WORD)
			{
				tmp->final_cmd[index.j]
					= ft_strdup(tmp->define[index.i].content);
				index.j++;
			}
		}
		tmp->final_cmd[index.j] = NULL;
		if (index.j == 0)
			tmp->final_cmd = NULL;
		tmp = tmp->next;
	}
}
