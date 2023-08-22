/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 20:29:26 by mzoheir           #+#    #+#             */
/*   Updated: 2023/08/16 14:05:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

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

char** removePipePointers(char** str) 
{
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

char* concatenate_char(char* str, char c) 
{
    int len;
    char* new_str; 
    
    len = ft_strlen(str);
    new_str = (char*)malloc(len + 2);
    if(!new_str)
        return(NULL);
    if (new_str) 
    {
        ft_memcpy(new_str, str, len); 
        new_str[len] = c;           
        new_str[len + 1] = '\0';
    }
    return new_str;
}

int checkQuoteIndex(char* str, int index) 
{
    int singleQuotes = 0;
    int doubleQuotes = 0;
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\') && doubleQuotes % 2 == 0) 
        {
            singleQuotes++;
            if (singleQuotes % 2 == 1 && i <= index)
                return 1;  // Index is between single quotes
        } 
        else if (str[i] == '"' && (i == 0 || str[i - 1] != '\\') && singleQuotes % 2 == 0)
        {
            doubleQuotes++;
            if (doubleQuotes % 2 == 1 && i <= index)
                return 0;  // Index is between double quotes
        }
        i++;
    }
    
    return 0;  // Index is not within quotes
}