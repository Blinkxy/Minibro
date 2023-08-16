/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:42:46 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/21 19:06:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"


t_list *createNode(char* cmd, int i) 
{
    t_list *newNode;
    int j;

    newNode = (t_list*)malloc(sizeof(t_list));
    if (!newNode)
		return NULL;
    newNode->index = i;
    newNode->cmd = split_cmd(cmd);
    j = 0;
    while(newNode->cmd[j])
        j++;
    newNode->size_cmd = j;
    newNode->next = NULL;
    return newNode;
}

void addNodeFront(t_list *head, char* str, int i) 
{
    t_list *current = head;

    while (current->next != NULL)
        current = current->next;
    current->next = createNode(str, i);
}
