#include "Minishell.h"

int countWords(char* str) 
{
    int count = 0;
    int inQuotes = 0;
    int wordStart = 0;
    int i = 0;

    while (str[i]) 
    {
        if (str[i] == '\'')
            inQuotes = !inQuotes;
        else if (str[i] == ' ' && !inQuotes) 
        {
            if (wordStart == 1) 
            {
                count++;
                wordStart = 0;
            }
        } 
        else
            wordStart = 1;
        i++;
    }
    if (wordStart == 1)
        count++;
    return count;
}

void fill_new_struct(char *str, t_define *new_struct)
{
    int i;
    char **split_words;
    int count_words;

    i = 0;
    split_words = splitWords(str,count_words);
    while(split_words[i])
    {
        new_struct[i].content = ft_strdup(split_words[i]);
        i++;
    }

}

void add_new_struct(t_define *original, t_define *to_add)
{



}

t_define *final_struct(t_list *cmds, char **env)
{
    t_list *tmp;
    int i;
    int j;
    char **split;
    t_define *new_struct;
    
    i = 0;
    j = 0;
    tmp = cmds;
    while(tmp)
    {
        while(i < tmp->define->size_struct)
        {
            if (tmp->define->dollar == 1)
            {
                tmp->define->content = expand_ENV(tmp->define->content, env);
                if (countWords(tmp->define->content) > 1)
                {
                    new_struct = malloc(sizeof(t_define) * countWords(tmp->define->content));
                    if(!new_struct)
                        return(NULL);
                    initialize_define(new_struct);
                    fill_new_struct(tmp->define->content,new_struct);
                    add_new_struct(tmp->define, new_struct);
                }
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
