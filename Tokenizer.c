/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:20:56 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/19 18:03:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void    tokenizer(char **str)
{
    int i;
    t_list *cmds;
    
    i = 0;
    cmds = createNode(str[i]);
    while(str[i])
    {
        if (ft_strnstr(str[i], "<", ft_strlen(str[i])))
        {
            cmds->type = RED_IN;
            cmds->index = i;
            cmds->len = ft_strlen(str[i]);
        }
        else if (ft_strnstr(str[i], "<<", ft_strlen(str[i])))
        {
            cmds->type = HEREDOC;
            cmds->index = i;
            cmds->len = ft_strlen(str[i]);
        }
        else if (ft_strnstr(str[i], ">", ft_strlen(str[i])))
        {
            cmds->type = RED_OUT;
            cmds->index = i;
            cmds->len = ft_strlen(str[i]);
        }
        else if (ft_strnstr(str[i], ">>", ft_strlen(str[i])))
        {
            cmds->type = APPEND;
            cmds->index = i;
            cmds->len = ft_strlen(str[i]);
        }
        else if (ft_strchr(str[i], '$') && ENV_checker(str[i]))
        {
            cmds->type = ENV;
            cmds->index = i;
            cmds->len = ft_strlen(str[i]);
        }
        if (ft_strchr(str[i], '\''))
            cmds->state = IN_SQUOTE;
        if (ft_strchr(str[i], '\"'))
            cmds->state = IN_DQUOTE;
        
        i++;
        cmds->next = createNode(str[i]);
    }
    free(str);
}

int    ENV_checker(char *str)
{
    int i;
    int inSquotes;
    int inDquotes;
    
    i = -1;
    while(str[++i])
    {
        if (str[i] == '\"' && inSquotes == 0 && inDquotes == 0)
            inDquotes = 1;
        else if (str[i] == '\"' && inDquotes == 1)
            inDquotes = 0;
        else if (str[i] == '\'' && inSquotes == 0 && inDquotes == 0)
            inSquotes = 1;
        else if (str[i] == '\'' && inSquotes == 1)
            inSquotes = 0;
        if (str[i] == '$')
        {
            if (inSquotes == 1 && inDquotes == 0)
                return(0);
            else if (inDquotes == 1 && inSquotes == 0)
                return(1);               
        }
    }
    return(0);
}

int cnt_cmds(char *line)
{
    int i;
    int open;
    int cmds;
    
    i = 0;
    open = 0;
    cmds = 0;
    while (line[i])
    {
        if (line[i] == '|' && open == 0)
            cmds++;
        else if (line[i] == '\"' && open == 0)
            open = 1;
        else if (line[i] == '\"' && open == 1)
            open = 0;
        else if (line[i] == '\'' && open == 0)
            open = 2;
        else if (line[i] == '\'' && open == 2)
            open = 0;
        i++;
    }
    return (cmds + 1);
}


