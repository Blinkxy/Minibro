/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:54:45 by mzoheir           #+#    #+#             */
/*   Updated: 2023/10/06 00:31:12 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int	checker_line(char *line)
{
	if (checkquotes(line) == 0)
	{
		printf("Invalid command: quotes not closed\n");
		return (0);
	}
	return (1);
}

int	error_handler(t_index_check *index)
{
	if (error_line_util(index) == 0)
		return (0);
	if (error_bis(index) == 0)
		return (0);
	if (error_line_bis(index) == 0)
		return (0);
	if (error_line_out(index) == 0)
		return (0);
	if (error_line_in(index) == 0)
		return (0);
	if (error_line_util_pipe(index) == 0)
		return (0);
	if (error_line_util_red(index) == 0)
		return (0);
	return (1);
}

int	checker_redir(char *line)
{
	t_index_check	index;

	initialize_checker(&index);
	index.copy = ft_strtrim(line, " ");
	if (index.copy[0] == '\0')
		return (0);
	index.len = ft_strlen(index.copy);
	if (error_line_end(&index) == 0)
		return (0);
	while (index.copy[index.i])
	{
		if (index.copy[index.i] == '\'' || index.copy[index.i] == '"')
			index.inquotes = !index.inquotes;
		if (error_handler(&index) == 0)
			return (0);
		index.i++;
	}
	free(index.copy);
	return (1);
}
