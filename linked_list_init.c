/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:42:46 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/15 20:22:20 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

t_list	*create_node(char *cmd, int i)
{
	t_list	*newNode;
	int		j;

	newNode = (t_list *)malloc(sizeof(t_list));
	if (!newNode)
		return (NULL);
	newNode->index = i;
	newNode->red_nb = 0;
	newNode->fd_in = 0;
	newNode->fd_out = 0;
	newNode->redir = 0;
	newNode->final_cmd = 0;
	newNode->cmd = split_cmd(cmd);
	j = 0;
	while (newNode->cmd[j])
		j++;
	newNode->size_cmd = j;
	newNode->next = NULL;
	newNode->prev = NULL;
	return (newNode);
}

void	add_node_front(t_list *head, char *str, int i)
{
	t_list	*current;

	current = head;
	while (current->next != NULL)
		current = current->next;
	current->next = create_node(str, i);
}

void	add_prev_list(t_list *cmds)
{
	t_list *tmp = cmds;
	t_list *prev = NULL;

	while (tmp)
	{
		tmp->prev = prev;
		prev = tmp;
		tmp = tmp->next;
	}
}