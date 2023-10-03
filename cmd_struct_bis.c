/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 06:34:15 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/24 01:02:53 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	count_word_bis(char *str, t_count *count_words, int *i)
{
	if (str[*i] == '\'')
	{
		if (!count_words->indoublequotes)
			count_words->insinglequotes = !count_words->insinglequotes;
		else
			count_words->wordstart = 1;
	}
	else if (str[*i] == '"')
	{
		if (!count_words->insinglequotes)
			count_words->indoublequotes = !count_words->indoublequotes;
		else
			count_words->wordstart = 1;
	}
	else if (str[*i] == ' ' && !count_words->insinglequotes
			&& !count_words->indoublequotes)
	{
		if (count_words->wordstart == 1)
		{
			count_words->word_count++;
			count_words->wordstart = 0;
		}
	}
	else
		count_words->wordstart = 1;
}

int	countwords(char *str)
{
	int		i;
	t_count	count;

	i = -1;
	initialize_counter(&count);
	while (str[++i])
	{
		count_word_bis(str, &count, &i);
	}
	if (count.wordstart == 1 && !count.insinglequotes && !count.indoublequotes)
		count.word_count++;
	return (count.word_count);
}
