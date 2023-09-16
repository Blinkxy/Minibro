/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:19:32 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/16 04:05:23 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int gb_ex_st = 0;


void default_fds(t_list *cmds)
{
    t_list *head;

    head = cmds;
    while(head)
    {
        head->fd[0] = -1;
        head->fd[1] = -1;
        head = head->next;
    }
}

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
                str = remove_pipe_pointers(str);
                i = 0;
                if(str[0])
                    cmds = create_node(str[0], i);
                while( ++i < count_cmds(str))
                    add_node_front(cmds, str[i],i);
               // add_prev_list(cmds);
                cmd_define(cmds);
                final_struct(cmds,env);
                final_remove_quotes(cmds);
                redir_array(cmds);
                final_cmd(cmds);
                default_fds(cmds);
                make_red(cmds, sa);
                ex_test(cmds, sa);
            }
        }
    }
    return(0);
}
