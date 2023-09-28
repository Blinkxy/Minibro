/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 22:06:09 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/28 22:29:45 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int	redir_array_bis(t_list *tmp)
{
	int	i;
	int	nb_red;

	i = -1;
	nb_red = 0;
	while (++i < tmp->size_cmd)
	{
		if (ft_strncmp("<<", tmp->define[i].content, 2) == 0 || ft_strncmp(">>",
				tmp->define[i].content, 2) == 0 || ft_strncmp("<",
				tmp->define[i].content, 1) == 0 || ft_strncmp(">",
				tmp->define[i].content, 1) == 0)
			nb_red++;
	}
	return (nb_red);
}

void	initialize_redir(t_list *tmp)
{
	tmp->redir->delimiter = NULL;
	tmp->redir->file = NULL;
	tmp->redir->type = 0;
}

void	redir_array(t_list *commands)
{
	int		i;
	int		j;
	int		nb_red;
	t_list	*tmp;

	tmp = commands;
	while (tmp)
	{
		nb_red = redir_array_bis(tmp);
		if (nb_red > 0)
		{
			tmp->red_nb = nb_red;
			tmp->redir = malloc(sizeof(t_redir) * nb_red);
			if (!tmp->redir)
				return ;
			initialize_redir(tmp);
			i = -1;
			j = 0;
			while (++i < tmp->size_cmd && j < nb_red)
				redir_arrayx(tmp, &i, &j);
		}
		tmp = tmp->next;
	}
}
