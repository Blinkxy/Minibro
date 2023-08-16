#include "Minishell.h"

void free_struct(t_define *define)
{
    int i;

    i = 0;
    while(i < define->size_struct)
    {
        if(define[i].content)
            free(define[i].content);
        i++;
    }
}

void initialize_define(t_list *cmds)
{
    int j = 0;
    while (j < cmds->size_cmd) 
    {
    cmds->define[j].state = 0;
    cmds->define[j].type = 0;
    cmds->define[j].dollar = 0;
    cmds->define[j].content = 0;
    cmds->define[j].index = 0;
    cmds->define[j].size_struct = cmds->size_cmd;
    cmds->define[j].size_struct_inserted = 0;
    j++;
    }
}

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

void fill_new_struct(char *str, t_define *new_struct, t_list *cmds)
{
    int i;
    char **split_words;
    int count_words;

    count_words = countWords(str);
    i = 0;
    split_words = splitWords(str,count_words);
    while(split_words[i])
    {
        new_struct[i].content = ft_strdup(split_words[i]);
        new_struct[i].state = WORD;
        new_struct[i].type = new_struct[i].state;
        new_struct[i].size_struct_inserted = count_words;
        i++;
    }
}

void insert_new_struct(t_define *define, t_define *inserted, t_list *cmds, int index)
{
    int i;
    int j;
    t_define *final_define;
    
    final_define = (t_define *)malloc(sizeof(t_define) * cmds->size_cmd);
    i = 0;
    while( i < index - 1)
    {
        final_define[i] = define[i];
        i++;
    }
    j = 0;
    while( j < inserted->size_struct_inserted)
    {
        final_define[i + j] = inserted[j];
        j++;
    }
    while( i < cmds->size_cmd)
    {
        final_define[i + j] = define[i];
        i++;
    }
    cmds->size_cmd = i + j;
    free_struct(define);
    free_struct(inserted);
    define = final_define;
}

void *final_struct(t_list *cmds, char **env)
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
        i = 0;
        while(i < tmp->size_cmd)
        {
            if (tmp->define[i].dollar == 1)
            {
                tmp->define[i].content = expand_ENV(tmp->define->content, env);
                if (countWords(tmp->define[i].content) > 1 && tmp->define[i].type == FYLE)
                {
                    printf("ambiguous redirect\n");
                    return NULL;
                }
                else if (countWords(tmp->define[i].content) > 1 && tmp->define[i].type != FYLE)
                {
                    new_struct = malloc(sizeof(t_define) * countWords(tmp->define[i].content));
                    if(!new_struct)
                        return(NULL);
                    initialize_define(new_struct);
                    fill_new_struct(tmp->define->content,new_struct, cmds);
                    insert_new_struct(tmp->define, new_struct, cmds, i);
                }
            }
            i++;
        }
        tmp = tmp->next;
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
                tmp->define[i].index = i;
            }
            else if (ft_strncmp(">>", tmp->cmd[i], 2) == 0)
            {
                tmp->define[i].state = APPEND;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
                tmp->define[i].index = i;
            }
            else if (ft_strncmp("<", tmp->cmd[i], 1) == 0)
            {
                tmp->define[i].state = RED_IN;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
                tmp->define[i].index = i;
            }
            else if (ft_strncmp(">", tmp->cmd[i], 1) == 0)
            {
                tmp->define[i].state = RED_OUT;
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].type = tmp->define[i].state;
                tmp->define[i].index = i;
            }
            else if (tmp->define[i - 1].state == RED_IN || tmp->define[i - 1].state == APPEND ||
                tmp->define[i - 1].state == RED_OUT)
            {
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].state = FYLE;
                tmp->define[i].type = tmp->define[i].state;
                tmp->define[i].index = i;
            }
            else if (tmp->define[i - 1].state == HEREDOC)
            {
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].state = DELIMITER;
                tmp->define[i].type = tmp->define[i].state;
                tmp->define[i].index = i;
            }
            else
            {
                tmp->define[i].content = ft_strdup(tmp->cmd[i]);
                tmp->define[i].state = WORD;
                tmp->define[i].type = tmp->define[i].state;
                tmp->define[i].index = i;
            }
            if (ft_strchr(tmp->cmd[i], '$') && tmp->define[i].state != DELIMITER
            && checkQuoteIndex(tmp->cmd[i], ft_strchr(tmp->cmd[i], '$')) == 0)
                tmp->define[i].dollar = 1;
            i++;
        }
        tmp = tmp->next;
    }
}
