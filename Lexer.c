/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 20:29:26 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/21 18:37:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int checker_line(char *line)
{
    int i;
    
    if (checkQuotes(line) == 0)
    {
        printf("Invalid command: quotes not closed\n");
        return(0);
    }
    // DOUBLE PIPE or END with PIPE
    i = 0;
    while(line[i])
    {
        if(line[i] == '|')
        {
            if (!line[i + 1] || line[i + 1] == '|')
            {
                printf("syntax error near unexpected token '|'\n");
                return(0);
            }
        }
        i++;
    }
    return(1);
}

int checkQuotes(char *line)
{
    int singleQuotes = 0;
    int doubleQuotes = 0;
    int i = 0;
    
    while (line[i] != '\0') 
    {  
        if (line[i] == '\'' && doubleQuotes % 2 == 0)
            singleQuotes++;
        else if (line[i] == '"' && singleQuotes % 2 == 0)
            doubleQuotes++;
        i++;
    }
    return (singleQuotes % 2 == 0 && doubleQuotes % 2 == 0);
}

char* addnext_pipe(char* str) 
{
    int withinDQuotes = 0;
    int withinSQuotes = 0;
    int length = ft_strlen(str);
    char* modifiedStr = (char*)ft_calloc((length * 2 + 1) * sizeof(char), 1);
    int newIndex = 0;
    int i = -1;
    while(++i < length) 
    {
        if (str[i] == '\'' && withinSQuotes == 0 && withinDQuotes == 0) 
        {
            withinSQuotes = 1;
            modifiedStr[newIndex++] = str[i];
        }
        else if (str[i] == '\"' && withinSQuotes == 0 && withinDQuotes == 0) 
        {
            withinDQuotes = 1;
            modifiedStr[newIndex++] = str[i];
        }
        if (str[i] == '\'' && withinSQuotes == 1 && withinDQuotes == 0) 
            withinSQuotes = 0;
        else if (str[i] == '\"' && withinSQuotes == 0 && withinDQuotes == 1) 
            withinDQuotes = 0;
        else if (withinSQuotes == 0 && withinDQuotes == 0 && str[i] == '|')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '|';
            modifiedStr[newIndex++] = '\n';
        }
        else 
            modifiedStr[newIndex++] = str[i];
    }
    free(str);
    return modifiedStr;
}

char** removePipePointers(char** str) {
    char** result;
    int count = 0;
    int i;
    int j;

    i = 0;
    while (str[i]) 
    {
        if (str[i][0] != '|')
            count++;
        i++;
    }
    result = (char **)malloc((count + 1) * sizeof(char*));
    i = 0;
    j = 0;
    while (str[i]) 
    {
        if (str[i][0] != '|') 
        {
            result[j] = str[i];
            j++;
        }
        i++;
    }
    result[j] = NULL;
    free(str);
    return result;
}