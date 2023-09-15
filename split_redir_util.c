/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_redir_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 21:47:45 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/15 21:49:43 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void    split_cmd_util(t_index *index, char *str, char *newstr)
{
    if (str[index->i] == '\'' || str[index->i] == '\"')
    {
			index->inquotes = !index->inquotes;
			newstr[index->index++] = str[index->i];
	}
	else if (!index->inquotes && str[index->i] == '>' && str[index->i + 1] != '>')
	{
		newstr[index->index++] = '\n';
		newstr[index->index++] = '>';
		newstr[index->index++] = '\n';
	}
	else if (!index->inquotes && str[index->i] == '>' && str[index->i + 1] == '>')
	{
		newstr[index->index++] = '\n';
		newstr[index->index++] = '>';
		newstr[index->index++] = '>';
		newstr[index->index++] = '\n';
		index->i++;
	}
	else if (!index->inquotes && str[index->i] == '<' && str[index->i + 1] != '<')
	{
		newstr[index->index++] = '\n';
		newstr[index->index++] = '<';
		newstr[index->index++] = '\n';
	}
	else if (!index->inquotes && str[index->i] == '<' && str[index->i + 1] == '<')
	{
		newstr[index->index++] = '\n';
		newstr[index->index++] = '<';
		newstr[index->index++] = '<';
		newstr[index->index++] = '\n';
		index->i++;
	}
}