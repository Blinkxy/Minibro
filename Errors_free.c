#include "Minishell.h"

void free_define_and_cmd(t_list *cmds)
{
    int i;
    t_list *tmp;

    tmp = cmds;
    while(tmp)
    {
        i = 0;
        while(i < tmp->size_cmd)
        {
            if(cmds->define[i].content)
                free(cmds->define[i].content);
            i++;
        }
        i = 0;
        while(cmds->cmd[i])
        {
            free(cmds->cmd[i]);
            i++;
        }
        if(cmds->cmd)
            free(cmds->cmd);
        tmp = tmp->next;
    }
}

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

int checker_line(char *line)
{
    int i;
    
    if (checkQuotes(line) == 0)
    {
        printf("Invalid command: quotes not closed\n");
        return(0);
    }
    // DOUBLE PIPE or END with PIPE
    i = 0;
    while(line[i])
    {
        if(line[i] == '|')
        {
            if (!line[i + 1] || line[i + 1] == '|')
            {
                printf("syntax error near unexpected token '|'\n");
                return(0);
            }
        }
        i++;
    }
    return(1);
}
