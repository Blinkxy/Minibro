/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/09/15 15:48:02 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/15 15:48:02 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	split_redirections(char *newstr, t_index *index, char *str)
{
	if (str[index->i] == '\'' || str[index->i] == '\"')
		index->inquotes = !index->inquotes;
	if (!index->inquotes && index->insinglequotes == 0 && str[index->i] == '>' && str[index->i
			+ 1] == '>')
		split_append(newstr, index);
	else if (!index->inquotes && index->insinglequotes == 0 && str[index->i] == '<' && str[index->i
			+ 1] == '<')
		split_heredoc(newstr, index);
	else if (!index->inquotes && index->insinglequotes == 0 && str[index->i] == '<' && str[index->i
			+ 1] != '<' && str[index->i + 1])
		split_red_out(newstr, index);
	else if (!index->inquotes && index->insinglequotes == 0 && str[index->i] == '>' && str[index->i
		+ 1] != '>' && str[index->i + 1])
		split_red_in(newstr, index);
	else if (iswhitespace(str[index->i]))
		newstr[index->index++] = '\n';
	else
		newstr[index->index++] = str[index->i];
}

char	**split_cmd(char *str)
{
	t_index	index;
	char	*newstr;

	initialize_index(&index);
	index.len = ft_strlen(str);
	newstr = (char *)ft_calloc((index.len * 2 + 1) * sizeof(char), 1);
	if (!newstr)
		return (NULL);
	index.i = -1;
	while (++(index.i) < index.len)
		split_redirections(newstr, &index, str);
	free(str);
	return (ft_split(newstr, '\n'));
}

void	final_remove_quotes(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	tmp = cmds;
	while (tmp)
	{
		i = -1;
		while (++i < tmp->size_cmd)
		{
			if (tmp->define[i].type != DELIMITER)
				tmp->define[i].content = expand_quotes(tmp->define[i].content);
		}
		tmp = tmp->next;
	}
}

void	expand_quotes_util(t_index *index, char *str, char *result)
{
	if (str[index->i] == '\'' && (index->i == 0 || str[index->i - 1] != '\\')
		&& index->indoublequotes % 2 == 0)
		index->insinglequotes = (index->insinglequotes + 1) % 2;
	else if (str[index->i] == '"' && (index->i == 0 || str[index->i
				- 1] != '\\') && index->insinglequotes % 2 == 0)
		index->indoublequotes = (index->indoublequotes + 1) % 2;
	else
	{
		if (!(str[index->i] == '\'' && index->insinglequotes)
			&& !(str[index->i] == '"' && index->indoublequotes))
		{
			result[index->j] = str[index->i];
			index->j++;
		}
	}
}

char	*expand_quotes(char *str)
{
	t_index	index;
	char	*result;

	initialize_index(&index);
	index.len = ft_strlen(str);
	result = (char *)malloc(index.len + 1);
	if (!result)
		return (NULL);
	while (str[index.i])
	{
		expand_quotes_util(&index, str, result);
		index.i++;
	}
	free(str);
	result[index.j++] = '\0';
	return (result);
}
