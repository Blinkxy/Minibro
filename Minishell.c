/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:19:32 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/14 19:24:18 by mzoheir          ###   ########.fr       */
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
    // t_list *tmp;
    t_general *sa = malloc(sizeof(t_general));
    sa->cmds = malloc(sizeof(t_list));
    memset(sa, 0, sizeof(t_general));

    init_env_data(sa, env);
    get_export_env(sa);
    while(1)
    {
        s = readline("minishell$>");
        // if(!s)
        // {
        //         //protect Crtl-D
        // }
        if(s && s[0])
        {
            add_history(s);
            if (checker_line(s) == 1 && checker_redir(s) == 1)
            {
                st = addnext_pipe(s);
                str = ft_split(st,'\n');
                str = removePipePointers(str);
                i = 0;
                if(str[0])
                    cmds = createNode(str[0], i);
                while( ++i < count_cmds(str))
                    addNodeFront(cmds, str[i],i);
                add_prev_list(cmds);
                cmd_define(cmds);
                final_struct(cmds,env);
                redir_array(cmds);
                final_remove_quotes(cmds);
                final_cmd(cmds);
                //check_heredoc(cmds);
                
                ex_minishell(cmds, sa);
              //  free_define_and_cmd(cmds)
                // handle_redir(cmds, sa);

            }
        }
    }
    return(0);
}
