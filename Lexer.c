/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 20:29:26 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/24 00:47:07 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	add_next_pipe_bis(char *new_str, t_index *index)
{
	new_str[index->index++] = '\n';
	new_str[index->index++] = '|';
	new_str[index->index++] = '\n';
}

void	addnext_pipe_util(t_index *index, char *str, char *new_str)
{
	if (str[index->i] == '\'' && index->insinglequotes == 0
		&& index->indoublequotes == 0)
	{
		index->insinglequotes = 1;
		new_str[index->index++] = str[index->i];
	}
	else if (str[index->i] == '\"' && index->insinglequotes == 0
		&& index->indoublequotes == 0)
	{
		index->indoublequotes = 1;
		new_str[index->index++] = str[index->i];
	}
	if (str[index->i] == '\'' && index->insinglequotes == 1
		&& index->indoublequotes == 0)
		index->insinglequotes = 0;
	else if (str[index->i] == '\"' && index->insinglequotes == 0
		&& index->indoublequotes == 1)
		index->indoublequotes = 0;
	else if (index->insinglequotes == 0 && index->indoublequotes == 0
		&& str[index->i] == '|')
		add_next_pipe_bis(new_str, index);
	else
		new_str[index->index++] = str[index->i];
}

char	*addnext_pipe(char *str)
{
	char	*new_str;
	t_index	index;

	initialize_index(&index);
	index.len = ft_strlen(str);
	new_str = (char *)ft_calloc((index.len * 2 + 1) * sizeof(char), 1);
	index.i = -1;
	while (++(index.i) < index.len)
	{
		addnext_pipe_util(&index, str, new_str);
	}
	free(str);
	return (new_str);
}

void	remove_pipe_bis(char **result, char **str, int i, int *j)
{
	if (str[i][0] != '|')
	{
		result[*j] = str[i];
		(*j)++;
	}
}

char	**remove_pipe_pointers(char **str)
{
	char	**result;
	int		count;
	int		i;
	int		j;

	i = -1;
	count = 0;
	while (str[++i])
	{
		if (str[i][0] != '|')
			count++;
	}
	result = (char **)malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		remove_pipe_bis(result, str, i, &j);
		i++;
	}
	result[j] = NULL;
	free(str);
	return (result);
}
