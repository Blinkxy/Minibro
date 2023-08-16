/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:19:32 by mzoheir           #+#    #+#             */
/*   Updated: 2023/08/16 13:59:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int main()
{
    int i;
    char **str;
    char *s;
    char *st;
    char **env;  // DOUZI : GIVE IT YOUR DATA
    t_list *cmds = NULL;
 
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
                cmd_define(cmds);
                final_struct(cmds,env);
                final_remove_quotes(cmds);
                redir_array(cmds);
                final_cmd(cmds);
                free_define_and_cmd(cmds);
            }
        }
    }
    return(0);
}
