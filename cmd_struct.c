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

void initialize_define(t_define *define, int size)
{
    int j = 0;
    while (j < size)
    {
    define[j].state = 0;
    define[j].type = 0;
    define[j].dollar = 0;
    define[j].content = 0;
    define[j].index = 0;
    define[j].size_struct = 0;
    define[j].size_struct_inserted = size;
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

void fill_new_struct(char *str, t_define *new_struct)
{
    int i;
    char **split_words;
    
    i = 0;
    split_words = ft_split(str, ' ');
    while(split_words[i])
    {
        new_struct[i].content = ft_strdup(split_words[i]);
        new_struct[i].state = WORD;
        new_struct[i].type = new_struct[i].state;
        i++;
    }
    i = 0;
    while(split_words[i])
    {
        free(split_words[i]);
        i++;
    }
    if(split_words)
        free(split_words);
}

void insert_new_struct(t_define *define, t_define *inserted, t_list *cmds, int index)
{
    int i;
    int j;
    t_define *final_define;
    
    final_define = (t_define *)malloc(sizeof(t_define) * cmds->size_cmd);
    i = 0;
    while( i < index)
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
    // cmds->size_cmd = i + j;
    define = final_define;
}

void final_struct(t_list *cmds, char **env)
{
    t_list *tmp;
    int i;
    t_define *new_struct;
    
    i = 0;
    tmp = cmds;
    while(tmp)
    {
        i = 0;
        while(i < tmp->size_cmd)
        {
            if (tmp->define[i].dollar == 1)
            {
                // tmp->define[i].content = Expand_quotes(tmp->define[i].content);
                // tmp->define[i].content = ft_strtrim(tmp->define[i].content, " ");
                // printf("%s\n", tmp->define[i].content);
                tmp->define[i].content = expand_ENV(tmp->define[i].content, env);
                // printf("Expended:%s\n", tmp->define[i].content);
                if (countWords(tmp->define[i].content) > 1 && tmp->define[i].type == FYLE)
                {
                    printf("ambiguous redirect\n");
                    return;
                }
                else if (countWords(tmp->define[i].content) > 1 && tmp->define[i].type != FYLE)
                {
                    // printf("BUG!\n");
                    new_struct = malloc(sizeof(t_define) * countWords(tmp->define[i].content));
                    if(!new_struct)
                        return;
                    initialize_define(new_struct, countWords(tmp->define[i].content));
                    fill_new_struct(tmp->define[i].content,new_struct);
                    insert_new_struct(tmp->define, new_struct, cmds, i);
                }
            }
            // printf("size:%d et index:%d\n",tmp->size_cmd, tmp->index);
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
        initialize_define(tmp->define, tmp->size_cmd);
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
            if (ft_strchr(tmp->cmd[i], '$') && tmp->define[i].state != DELIMITER)
                tmp->define[i].dollar = 1;
            i++;
        }
        tmp = tmp->next;
    }
}

int check_dollar(char *str, int index)
{
    int i;
    int Squote;
    int Dquote;

    Squote = 0;
    Dquote = 0;
    i = 0;
    while(i < index)
    {
        if ( str[i] == '\''  && str[i - 1] != '\\' && Dquote == 0 && Squote == 0)
            Squote = 1;
        else if (str[i] == '\"' && str[i - 1] != '\\' && Dquote == 0 && Squote == 0)
            Dquote = 1;
        else if (str[i] == '\''  && str[i - 1] != '\\' && Dquote == 0 && Squote == 1)
            Squote = 0;
        else if (str[i] == '\"' && str[i - 1] != '\\' && Dquote == 1 && Squote == 0)
            Dquote = 0;
        i++;
    }
    if (i == index && Dquote == 1 && Squote == 0)
        return(0); // Inside Double Quotes
    else if ( i == index && Dquote == 0 && Squote == 0)
        return(0); // Not within Quotes
    else if ( i == index && Dquote == 0 && Squote == 1)
        return(1); // Inside Single Quotes
    return(0);
}