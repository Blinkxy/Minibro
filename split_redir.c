#include "Minishell.h"

int isWhitespace(char c) 
{
    return (c == ' ' || c == '\t');
}

char** split_cmd(char* str)
{
    int withinQuotes = 0;
    int length = ft_strlen(str);
    char* modifiedStr = (char*)ft_calloc((length * 2 + 1) * sizeof(char), 1);
    int newIndex = 0;
    int i = -1;
    while(++i < length) 
    {
        if (str[i] == '\'' || str[i] == '\"')
        {
            withinQuotes = !withinQuotes;
            modifiedStr[newIndex++] = str[i];
        }
        else if (!withinQuotes && str[i] == '>' && str[i + 1] != '>')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '>';
            modifiedStr[newIndex++] = '\n';
        }
        else if (!withinQuotes && str[i] == '>' && str[i + 1] == '>')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '>';
            modifiedStr[newIndex++] = '>';
            modifiedStr[newIndex++] = '\n';
            i++;
        }
        else if (!withinQuotes && str[i] == '<' && str[i + 1] != '<')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '<';
            modifiedStr[newIndex++] = '\n';
        }
        else if (!withinQuotes && str[i] == '<' && str[i + 1] == '<')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '<';
            modifiedStr[newIndex++] = '<';
            modifiedStr[newIndex++] = '\n';
            i++;
        }
        else if (!withinQuotes && isWhitespace(str[i])) 
            modifiedStr[newIndex++] = '\n';
        else 
            modifiedStr[newIndex++] = str[i];
    }
    free(str);
    return (ft_split(modifiedStr, '\n'));
}

int count_cmds(char **str)
{
    int i;

    i = 0;
    while(str[i])
        i++;
    return(i);
}

// char **split_red(t_list *cmds)
// {
//     int i;

//     i = 0;
//     while(str[i])
//     {

//     }
// }
