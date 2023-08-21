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

void init_env_data(t_general *sa, char **envp)
{
    int i;

    i  = 0;
    sa->env = malloc(sizeof(char *) * (ft_size(envp) + 1));
    while(envp[i])
    {
        sa->env[i] = ft_strdup(envp[i]);
        i++;
    }
    sa->env[i] = NULL;
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    int i;
    char **str;
    char *s;
    char *st;
    t_list *cmds = NULL;
    t_list *tmp;
    t_general *sa = malloc(sizeof(t_general));
    sa->cmds = malloc(sizeof(t_list));
    memset(sa, 0, sizeof(t_general));

    init_env_data(sa, env);
    get_export_env(sa);
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

                // free_define_and_cmd(cmds);
                
                
                // if_builtin(sa->cmds->final_cmd, sa, 1);
            }
        }
    }
    return(0);
}
