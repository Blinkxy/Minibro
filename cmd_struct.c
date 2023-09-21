/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:54:36 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/15 15:54:36 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int	skip_quote(char *str, int i)
{
	if (str[i] == '\"')
	{
		i++;
		while (str[i] != '\"')
			i++;
	}
	else if (str[i] == '\'')
	{
		i++;
		while (str[i] != '\'')
			i++;
	}
	return (i);
}
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

char	*fix_split(char *str)
{
	int		i;
	int		len;
	char	*fixed;

	i = 0;
	len = ft_strlen(str);
	fixed = malloc(sizeof(char) * len);
	while (i < len - 1)
	{
		fixed[i] = str[i];
		i++;
	}
	fixed[len - 1] = '\0';
	return (fixed);
}

void	fill_new_struct(char *str, t_define *new_struct)
{
	int		i;
	char	**split_words;
	char	*copy;

	i = 0;
	copy = ft_strdup(str);
	while (copy[i])
	{
		if (copy[i] == '\"' || copy[i] == '\'')
			i = skip_quote(copy, i);
		else if (copy[i] == ' ' || copy[i] == '\t' || copy[i] == '\n')
			copy[i] = '\n';
		i++;
	}
	i = 0;
	split_words = ft_split(copy, '\n');
	while (split_words[i])
	{
		new_struct[i].content = ft_strdup(split_words[i]);
		new_struct[i].state = WORD;
		new_struct[i].type = new_struct[i].state;
		i++;
	}
	free_double_array(split_words);
}

void	free_double_array(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

t_define	*insert_new_struct(t_define *define, t_define *inserted,
		t_list *cmds, int index)
{
	int			i;
	int			j;
	t_define	*final_define;

	final_define = (t_define *)malloc(sizeof(t_define) * cmds->size_cmd);
	i = 0;
	while (i < index)
	{
		final_define[i] = define[i];
		i++;
	}
	j = 0;
	while (j < inserted->size_struct_inserted)
	{
		final_define[i + j] = inserted[j];
		j++;
	}
	while (i < (cmds->size_cmd) - (inserted->size_struct_inserted) + 1)
	{
		final_define[i + j] = define[i];
		i++;
	}
	return (final_define);
}

void	final_struct(t_list *cmds, char **env)
{
	t_list		*tmp;
	int			i;
	t_define	*new_struct;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		i = 0;
		while (i < tmp->size_cmd)
		{
			if (tmp->define[i].dollar == 1)
			{
				tmp->define[i].content = expand_env(tmp->define[i].content,
													env);
				if (countwords(tmp->define[i].content) > 1
					&& tmp->define[i].type == FYLE)
				{
					printf("ambiguous redirect\n");
					// free all
					break ;
				}
				else if (countwords(tmp->define[i].content) > 1
						&& tmp->define[i].type != FYLE)
				{
					new_struct = malloc(sizeof(t_define)
							* countwords(tmp->define[i].content));
					if (!new_struct)
					{
						// free all
						break ;
					}
					initialize_define_inserted(new_struct,
												countwords(tmp->define[i].content));
					fill_new_struct(tmp->define[i].content, new_struct);
					tmp->size_cmd += countwords(tmp->define[i].content) - 1;
					tmp->define = insert_new_struct(tmp->define, new_struct,
							tmp, i);
					i += countwords(tmp->define[i].content) - 1;
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
}

int	check_dollar(char *str, int index)
{
	t_index utils;

	initialize_index(&utils);
	while (utils.i < index)
	{
		if (str[utils.i] == '\'' && str[utils.i - 1] != '\\'
			&& utils.indoublequotes == 0 && utils.insinglequotes == 0)
			utils.insinglequotes = 1;
		else if (str[utils.i] == '\"' && str[utils.i - 1] != '\\'
				&& utils.indoublequotes == 0 && utils.insinglequotes == 0)
			utils.indoublequotes = 1;
		else if (str[utils.i] == '\'' && str[utils.i - 1] != '\\'
				&& utils.indoublequotes == 0 && utils.insinglequotes == 1)
			utils.insinglequotes = 0;
		else if (str[utils.i] == '\"' && str[utils.i - 1] != '\\'
				&& utils.indoublequotes == 1 && utils.insinglequotes == 0)
			utils.indoublequotes = 0;
		utils.i++;
	}
	if (utils.i == index && utils.indoublequotes == 1
		&& utils.insinglequotes == 0)
		return (0); // Inside Double Quotes
	else if (utils.i == index && utils.indoublequotes == 0
			&& utils.insinglequotes == 0)
		return (0); // Not within Quotes
	else if (utils.i == index && utils.indoublequotes == 0
			&& utils.insinglequotes == 1)
		return (1); // Inside Single Quotes
	return (0);
}