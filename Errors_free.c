/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:54:45 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/25 00:08:04 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	free_define_and_cmd(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	tmp = cmds;
	while (tmp)
	{
		i = -1;
		while (++i < tmp->size_cmd)
		{
			if (cmds->define[i].content)
				free(cmds->define[i].content);
		}
		if (tmp->define)
			free(tmp->define);
		i = -1;
		if (cmds->cmd && cmds->cmd[i])
		{
			while (++i < tmp->size_cmd)
				free(cmds->cmd[i]);
			free(cmds->cmd);
		}
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
	free(define);
}

int	checker_line(char *line)
{
	if (checkquotes(line) == 0)
	{
		printf("Invalid command: quotes not closed\n");
		return (0);
	}
	return (1);
}

void	initialize_checker(t_index_check *index)
{
	index->i = 0;
	index->j = 0;
	index->len = 0;
	index->inquotes = 0;
	index->copy = NULL;
}

int	checker_redir(char *line)
{
	t_index_check	index;

	initialize_checker(&index);
	index.copy = ft_strtrim(line, " ");
	index.len = ft_strlen(index.copy);
	if (error_line_end(&index) == 0)
		return (0);
	while (index.copy[index.i])
	{
		if (index.copy[index.i] == '\'' || index.copy[index.i] == '"')
			index.inquotes = !index.inquotes;
		if (error_line_util(&index) == 0)
			return (0);
		if (error_bis(&index) == 0)
			return (0);
		if (error_line_bis(&index) == 0)
			return (0);
		if (error_line_out(&index) == 0)
			return (0);
		if (error_line_in(&index) == 0)
			return (0);
		index.i++;
	}
	free(index.copy);
	return (1);
}
