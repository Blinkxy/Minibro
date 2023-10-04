/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:13:53 by mzoheir           #+#    #+#             */
/*   Updated: 2023/10/04 04:44:30 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

char	*concatenate_char(char *str, char c)
{
	int		i;
	int		len;
	char	*new_str;

	i = 0;
	len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

void	initialize_lex(t_lexer *index)
{
	index->count = 0;
	index->i = 0;
	index->j = 0;
	index->result = NULL;
}

void	addnext_pipe_bis(t_index *index, char *new_str)
{
	new_str[index->index++] = '\n';
	new_str[index->index++] = '|';
	new_str[index->index++] = '\n';
}

void	initialize_checker(t_index_check *index)
{
	index->i = 0;
	index->j = 0;
	index->len = 0;
	index->inquotes = 0;
	index->copy = NULL;
}
