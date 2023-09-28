/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_struct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:54:36 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/28 17:34:31 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

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

int	final_struct_util(t_list *tmp, int *i, t_define *new_struct, t_general *sa)
{
	if (countwords(tmp->define[*i].content) > 1 && tmp->define[*i].type == FYLE)
	{
		printf("ambiguous redirect\n");
		sa->ex_status = 1;
		return (1);
	}
	else if (countwords(tmp->define[*i].content) > 1
		&& tmp->define[*i].type != FYLE)
	{
		new_struct = malloc(sizeof(t_define)
				* countwords(tmp->define[*i].content));
		if (!new_struct)
			return (-1);
		initialize_define_inserted(new_struct,
			countwords(tmp->define[*i].content));
		fill_new_struct(tmp->define[*i].content, new_struct);
		tmp->size_cmd += countwords(tmp->define[*i].content) - 1;
		tmp->define = insert_new_struct(tmp->define, new_struct, tmp, *i);
		*i += countwords(tmp->define[*i].content) - 1;
	}
	return (0);
}

void	final_struct(t_list *cmds, char **env, t_general *sa)
{
	t_list		*tmp;
	int			i;
	t_define	*new_struct;

	tmp = cmds;
	while (tmp)
	{
		i = 0;
		new_struct = NULL;
		while (i < tmp->size_cmd)
		{
			if (tmp->define[i].dollar == 1)
			{
				tmp->define[i].content = expand_env(tmp->define[i].content,
						env, sa);
				if (final_struct_util(tmp, &i, new_struct, sa) == -1)
					break ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}
