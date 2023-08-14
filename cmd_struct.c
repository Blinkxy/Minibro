#include "Minishell.h"

t_define *final_struct(t_list *cmds, char **env)
{
    t_list *tmp;
    int i;
    char **split;
    i = 0;
    tmp = cmds;
    while(tmp)
    {
        while(i < tmp->define->size_struct)
        {
            if (tmp->define->dollar == 1)
                tmp->define->content = expand_ENV(tmp->define->content, env);
            if (count_strings(tmp->define->content) > 1)
            {
                split = ft_split(tmp->define->content,' ');
                tmp->define->size_struct = tmp->size_cmd +
                count_strings(tmp->define->content) - 1;
            }
            i++;
        }
        tmp = tmp->next;
    }
}


void initialize_define(t_list *cmds)
{
    int j = 0;
    while (j < cmds->size_cmd) 
    {
    cmds->define[j].state = 0; // Set an appropriate default value
    cmds->define[j].type = 0; // Set an appropriate default value
    cmds->define[j].dollar = 0; // Set to a default value
    cmds->define[j].content = 0; // Set to a default value
    cmds->define[j].size_struct = 0; // Set to a default value
    j++; // Move to the next index
    }
}


void cmd_define(t_list *cmds)
{
    int i = 0;
    t_list *tmp;

    tmp = cmds;
    while(tmp)
    {
        tmp->define = malloc(sizeof(t_define) * (tmp->size_cmd));
        if(!(tmp->define))
            return;
        initialize_define(tmp);
        i = 0;
        while(tmp->cmd[i])
        {
            if (ft_strncmp("<<", tmp->cmd[i], 2) == 0)
            {
                tmp->define[i].state = HEREDOC;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
            }
            else if (ft_strncmp(">>", tmp->cmd[i], 2) == 0)
            {
                tmp->define[i].state = APPEND;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
            }
            else if (ft_strncmp("<", tmp->cmd[i], 1) == 0)
            {
                tmp->define[i].state = RED_IN;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
            }
            else if (ft_strncmp(">", tmp->cmd[i], 1) == 0)
            {
                tmp->define[i].state = RED_OUT;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
            }
            else if (tmp->define[i - 1].state == RED_IN || tmp->define[i - 1].state == APPEND ||
                tmp->define[i - 1].state == RED_OUT)
            {
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].state = FYLE;
                tmp->define[i].type = tmp->define[i].state;
            }
            else if (tmp->define[i - 1].state == HEREDOC)
            {
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].state = DELIMITER;
                tmp->define[i].type = tmp->define[i].state;
            }
            else
            {
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].state = WORD;
                tmp->define[i].type = tmp->define[i].state;
            }
            if (ft_strchr(tmp->cmd[i], '$') && tmp->define[i].state != DELIMITER
            && checkQuoteIndex(tmp->cmd[i], ft_strchr(tmp->cmd[i], '$')) == 0)
                tmp->define[i].dollar = 1;
            i++;
        }
        tmp = tmp->next;
    }
}
