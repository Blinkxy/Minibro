/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_redir_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 21:47:45 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/17 02:46:04 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	split_red_in(char *newstr, t_index *index)
{
	newstr[index->index++] = '\n';
	newstr[index->index++] = '>';
	newstr[index->index++] = '\n';
}

void	split_append(char *newstr, t_index *index)
{
	newstr[index->index++] = '\n';
	newstr[index->index++] = '>';
	newstr[index->index++] = '>';
	newstr[index->index++] = '\n';
	index->i++;
}

void	split_red_out(char *newstr, t_index *index)
{
	newstr[index->index++] = '\n';
	newstr[index->index++] = '<';
	newstr[index->index++] = '\n';
}

void	split_heredoc(char *newstr, t_index *index)
{
	newstr[index->index++] = '\n';
	newstr[index->index++] = '<';
	newstr[index->index++] = '<';
	newstr[index->index++] = '\n';
	index->i++;
}
