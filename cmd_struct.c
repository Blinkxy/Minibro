/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:54:36 by mzoheir           #+#    #+#             */
/*   Updated: 2023/10/03 06:07:43 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	final_struct(t_list *cmds, char **env, t_general *sa)
{
	t_list	*tmp;
	int		i;

	tmp = cmds;
	while (tmp)
	{
		i = -1;
		while (++i < tmp->size_cmd)
		{
			if (tmp->define[i].dollar == 1)
			{
				tmp->define[i].content = expand_env(tmp->define[i].content,
													env,
													sa);
			}
		}
		tmp = tmp->next;
	}
}

char	*filler_split(char *str)
{
	int		i;
	char	*res;
	int		j;

	res = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			j = i;
			j = skip_quote(str, i);
			while (i++ < j)
				res[i] = str[i];
		}
		else if (str[i] == ' ' || str[i] == '\t')
			res[i] = '\n';
		else
			res[i] = str[i];
		i++;
	}
	res[ft_strlen(str)] = '\0';
	return (res);
}

void	fill_new_split(char *str, t_define *new_struct, int *index, int k)
{
	int		i;
	char	**split_words;
	char	*copy;

	copy = filler_split(str);
	split_words = ft_split(copy, '\n');
	i = 0;
	while (split_words[i])
	{
		new_struct[*index].content = ft_strdup(split_words[i]);
		new_struct[*index].state = WORD;
		new_struct[*index].index = *index;
		new_struct[*index].type = new_struct[i].state;
		new_struct[*index].size_struct = k;
		(*index)++;
		i++;
	}
	free_tab(split_words);
}

int	new_struct_size(t_list *tmp)
{
	int	i;
	int	k;

	i = -1;
	k = 0;
	while (++i < tmp->size_cmd)
		k += countwords(tmp->define[i].content);
	return (k);
}

void	update_struct(t_list *cmds, t_general *sa)
{
	int			k;
	int			i;
	int			index;
	t_list		*tmp;
	t_define	*final_struct;

	tmp = cmds;
	while (tmp)
	{
		k = new_struct_size(tmp);
		final_struct = (t_define *)malloc(sizeof(t_define) * k);
		initialize_define(final_struct, k);
		i = -1;
		index = 0;
		while (++i < tmp->define->size_struct)
		{
			if (tmp->define[i].content[0] != '\0')
			{
				if (countwords(tmp->define[i].content) > 1
					&& tmp->define[i].type != FYLE)
					fill_new_split(tmp->define[i].content, final_struct, &index,
							k);
				else if (countwords(tmp->define[i].content) > 1
						&& tmp->define[i].type == FYLE)
				{
					printf("ambiguous redirect\n");
					sa->ex_status = 1;
					tmp->size_cmd = i;
					break ;
				}
				else if (countwords(tmp->define[i].content) == 1)
				{
					final_struct[index].content = ft_strdup(tmp->define[i].content);
					final_struct[index].type = tmp->define[i].type;
					final_struct[index].state = tmp->define[i].state;
					final_struct[index].index = index;
					final_struct[index].size_struct = k;
					index++;
				}
			}
		}
		free_struct(tmp);
		tmp->define = final_struct;
		tmp = tmp->next;
	}
}
