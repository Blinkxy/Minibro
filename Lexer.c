/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 20:29:26 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/18 20:02:28 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int	checkquotes(char *line)
{
	int	singlequotes;
	int	doublequotes;
	int	i;

	singlequotes = 0;
	doublequotes = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' && doublequotes % 2 == 0)
			singlequotes++;
		else if (line[i] == '"' && singlequotes % 2 == 0)
			doublequotes++;
		i++;
	}
	return (singlequotes % 2 == 0 && doublequotes % 2 == 0);
}

void	addnext_pipe_util(t_index *index, char *str, char *new_str)
{
	if (str[index->i] == '\'' && index->insinglequotes == 0 && index->indoublequotes == 0)
	{
		index->insinglequotes = 1;
		new_str[index->index++] = str[index->i];
	}
	else if (str[index->i] == '\"' && index->insinglequotes == 0 && index->indoublequotes == 0)
	{
		index->indoublequotes = 1;
		new_str[index->index++] = str[index->i];
	}
	if (str[index->i] == '\'' && index->insinglequotes == 1 && index->indoublequotes == 0)
		index->insinglequotes = 0;
	else if (str[index->i] == '\"' && index->insinglequotes == 0 && index->indoublequotes == 1)
		index->indoublequotes = 0;
	else if (index->insinglequotes == 0 && index->indoublequotes == 0 && str[index->i] == '|')
	{
		new_str[index->index++] = '\n';
		new_str[index->index++] = '|';
		new_str[index->index++] = '\n';
	}
	else
		new_str[index->index++] = str[index->i];
}

char	*addnext_pipe(char *str)
{
	char	*new_str;
	t_index index;

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
	i = -1;
	j = -1;
	while (str[++i])
	{
		if (str[i][0] != '|')
			result[++j] = str[i];
	}
	result[j] = NULL;
	free(str);
	return (result);
}

char	*concatenate_char(char *str, char c)
{
	int		len;
	char	*new_str;

	len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	if (new_str)
	{
		ft_memcpy(new_str, str, len);
		new_str[len] = c;
		new_str[len + 1] = '\0';
	}
	return (new_str);
}

int	checkquote_index(char *str, int index)
{
	int singlequotes = 0;
	int doublequotes = 0;
	int i = 0;

	while (str[i] != '\0')
	{
		if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\') && doublequotes
			% 2 == 0)
		{
			singlequotes++;
			if (singlequotes % 2 == 1 && i <= index)
				return (1);
		}
		else if (str[i] == '"' && (i == 0 || str[i - 1] != '\\') && singlequotes
				% 2 == 0)
		{
			doublequotes++;
			if (doublequotes % 2 == 1 && i <= index)
				return (0);
		}
		i++;
	}
	return (0);
}
