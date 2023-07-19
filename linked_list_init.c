/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:42:46 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/19 21:08:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"


t_list *createNode(char* cmd, int i) 
{
    t_list *newNode;
    
    newNode = (t_list*)malloc(sizeof(t_list));
    if (!newNode)
		return NULL;
    newNode->raw_cmd = cmd;
    newNode->index = i;
    newNode->cmd = split_cmd(cmd);
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

void freeLinkedList(t_list *head) 
{
    t_list *temp = head;
    int i;

    i = 0;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        while(temp)
        {
            free(temp->cmd[i]);
            temp = temp->next;
        }
        free(temp->cmd);
        free(temp->raw_cmd);
        free(temp);
    }
}

// t_list *splitString(char* str) 
// {
//     t_list *head = NULL;
//     int len = strlen(str);
//     int i = 0;
//     int wordStart = 0;

//     while (i < len)
//     {
//         if (str[i] == '\n')
//         {
//             int wordLength = i - wordStart;
//             if (wordLength > 0) 
//             {
//                 char* word = (char*)malloc((wordLength + 1) * sizeof(char));
//                 strncpy(word, &str[wordStart], wordLength);
//                 word[wordLength] = '\0';
//                 addNodeFront(head, word);
//             }
//             wordStart = i + 1;
//         }
//         i++;
//     }

//     // Handle the last word if it exists
//     int lastWordLength = i - wordStart;
//     if (lastWordLength > 0) 
//     {
//         char* lastWord = (char*)malloc((lastWordLength + 1) * sizeof(char));
//         strncpy(lastWord, &str[wordStart], lastWordLength);
//         lastWord[lastWordLength] = '\0';
//         addNodeFront(head, lastWord);
//     }

//     return head;
// }

// void printLinkedList(t_list *head) 
// {
//     while (head != NULL) {
//         printf("%s\n", head->command);
//         head = head->next;
//     }
// }

// int main() {
//     char str[] = "This\nis\na\nsample\nstring\nwith\n'single\nquotes'\nand\n\"double\nquotes\".";

//     printf("Original string:\n%s\n", str);
//     t_list *head = splitString(str);
//     printf("\nWords in the linked list:\n");
//     printLinkedList(head);

//     // Free memory
//     freeLinkedList(head);

//     return 0;
// }
