#include "Minishell.h"

t_redir *redir_array(t_list *commands)
{
    int i;
    int j = 0;
    int nb_red;
    t_list *tmp;
    tmp = commands;

    while(tmp)
    {
        i = 0;
        nb_red = 0;
        while(i < tmp->size_cmd)
        {
            if (ft_strncmp("<<", tmp->define[i].content, 2) == 0 || ft_strncmp(">>", tmp->define[i].content, 2) == 0 ||
                ft_strncmp("<", tmp->define[i].content, 1) == 0 || ft_strncmp(">", tmp->define[i].content, 1) == 0)
                nb_red++;
            i++;
        }
        if (nb_red > 0)
        {
            tmp->redir = malloc(sizeof(t_redir) * nb_red);
            i = 0;
            j = 0;
            while(i < tmp->size_cmd && j < nb_red)
            {
                if(ft_strncmp("<<", tmp->define[i].content, 2) == 0)
                {
                    tmp->redir[j].type = HEREDOC;
                    tmp->redir[j].red = ft_strdup("<<");
                    tmp->redir[j].delimiter = ft_strdup(tmp->define[i + 1].content);
                    j++;
                }
                else if(ft_strncmp(">>", tmp->define[i].content, 2) == 0)
                {
                    tmp->redir[j].type = APPEND;
                    tmp->redir[j].red = ft_strdup(">>");
                    tmp->redir[j].file = ft_strdup(tmp->define[i + 1].content);
                    j++;
                }
                else if(ft_strncmp(">", tmp->define[i].content, 1) == 0)
                {
                    tmp->redir[j].type = RED_OUT;
                    tmp->redir[j].red = ft_strdup(">");
                    tmp->redir[j].file = ft_strdup(tmp->define[i + 1].content);
                    j++;
                }
                else if(ft_strncmp("<", tmp->define[i].content, 1) == 0)
                {
                    tmp->redir[j].type = RED_IN;
                    tmp->redir[j].red = ft_strdup("<");
                    tmp->redir[j].file = ft_strdup(tmp->define[i + 1].content);
                    j++;
                }
            i++;
            }
        }
        tmp = tmp->next;
    }
}
