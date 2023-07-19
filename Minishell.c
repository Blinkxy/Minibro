/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:19:32 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/19 21:57:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int start_pipe(char *s)
{
    int i  = 0;
    
    if(s[0] == '|')
        return(0);
    while(s[i] == ' ' || s[i] == '\t')
        i++;
    if (s[i] == '|')
        return(0);
    return(1);    
}

int main()
{
    int i;
    char **str;
    char *s;
    char *st;
    t_list *cmds = NULL;
    t_list *tmp = NULL;
    while(1)
    {
        s = readline("mzoheir$>");
        // if(!s)
        // {
        //         //protect Crtl-D
        // }
        if(s && s[0])
        {
            add_history(s);
            if (start_pipe(s) == 0)
                printf("syntax error near unexpected token '|'\n");
            if (checker_line(s) == 1 && start_pipe(s) == 1)
            {
                st = addnext_pipe(s);
                str = ft_split(st,'\n');
                str = removePipePointers(str);

                i = 0;
                if(str[0])
                    cmds = createNode(str[0], i);
                while( ++i < count_cmds(str))
                    addNodeFront(cmds, str[i],i);
                
                
                
                
                
                tmp = cmds;
                while(tmp)
                {
                    int j = 0;
                    while (tmp->cmd[j])
                    {
                        // printf("node:%d\n",tmp->index);
                        printf("%s\n", tmp->cmd[j]);
                        j++;
                    }
                    tmp = tmp->next;
                }
                
                
                  
            }
        }
    }
    return(0);
}
