/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 00:40:09 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/25 21:29:09 by mzoheir          ###   ########.fr       */
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

int	checkquote_index(char *str, int index)
{
	t_index	index_q;

	initialize_index(&index_q);
	while (str[index_q.i] != '\0')
	{
		if (str[index_q.i] == '\'' && (index_q.i == 0
				|| str[index_q.i - 1] != '\\')
			&& index_q.indoublequotes % 2 == 0)
		{
			index_q.insinglequotes++;
			if (index_q.insinglequotes % 2 == 1 && index_q.i <= index)
				return (1);
		}
		else if (str[index_q.i] == '"' && (index_q.i == 0 || str[index_q.i
					- 1] != '\\') && index_q.insinglequotes % 2 == 0)
		{
			index_q.indoublequotes++;
			if (index_q.indoublequotes % 2 == 1 && index_q.i <= index)
				return (0);
		}
		index_q.i++;
	}
	return (0);
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

void	initialize_main(t_main *main, t_general *sa)
{
	main->i = 0;
	main->str = NULL;
	main->s = NULL;
	main->st = NULL;
	sa->ex_status = 0;
}
