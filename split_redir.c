#include "Minishell.h"

int isWhitespace(char c) 
{
    return (c == ' ' || c == '\t');
}

char** split_cmd(char* str)
{
    int withinQuotes = 0;
    int length = ft_strlen(str);
    char* modifiedStr = (char*)ft_calloc((length * 2 + 1) * sizeof(char), 1);
    int newIndex = 0;
    int i = -1;
    while(++i < length) 
    {
        if (str[i] == '\'' || str[i] == '\"')
        {
            withinQuotes = !withinQuotes;
            modifiedStr[newIndex++] = str[i];
        }
        else if (!withinQuotes && str[i] == '>' && str[i + 1] != '>')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '>';
            modifiedStr[newIndex++] = '\n';
        }
        else if (!withinQuotes && str[i] == '>' && str[i + 1] == '>')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '>';
            modifiedStr[newIndex++] = '>';
            modifiedStr[newIndex++] = '\n';
            i++;
        }
        else if (!withinQuotes && str[i] == '<' && str[i + 1] != '<')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '<';
            modifiedStr[newIndex++] = '\n';
        }
        else if (!withinQuotes && str[i] == '<' && str[i + 1] == '<')
        {
            modifiedStr[newIndex++] = '\n';
            modifiedStr[newIndex++] = '<';
            modifiedStr[newIndex++] = '<';
            modifiedStr[newIndex++] = '\n';
            i++;
        }
        else if (!withinQuotes && isWhitespace(str[i])) 
            modifiedStr[newIndex++] = '\n';
        else 
            modifiedStr[newIndex++] = str[i];
    }
    free(str);
    return (ft_split(modifiedStr, '\n'));
}

int count_cmds(char **str)
{
    int i;

    i = 0;
    while(str[i])
        i++;
    return(i);
}

char *get_ENV(char **env, char *check)
{
    int i;
    int j;
    char *str;

    i = 0;
    while(env[i])
    {
        if(compare_env_var(env[i], check) == 1)
        {
            j = 0;
            while(env[i][j] && env[i][j] != '=')
                j++;
            str = ft_substr(env[i], j, ft_strlen(env[i]) - j);
            return(str);
        }
        i++;     
    }
    return(0);
}

char *extract_ENV(char *str)
{
    int i;

    i = 0;
    while(str[i] && str[i] != '=')
        i++;
    if(str[i] == '=')
        i++;
    return(ft_substr(str,i,ft_strlen(str) - i));
}

char *expand_ENV(char *str, char **env)
{
    int i;
    int j;
    char *result;
    char *check_env;
    char *extract;

    result = ft_calloc(1, 1);
    if (!result)
        return NULL;
    i = 0;
    while (str[i])
    {
        if (str[i] == '$' && checkQuoteIndex(str, &str[i]) == 0)
        {
            j = i + 1;
            while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
                j++;
            if (j > i + 1)
                check_env = ft_substr(str, i + 1, j - i);
            if (get_env_var(env, check_env) == -1 && check_env)
                i += ft_strlen(check_env);
            else if (get_env_var(env, check_env) != -1 && check_env)
            {
                extract = extract_ENV(env[get_env_var(env, check_env)]);
                result = ft_strjoin(result, extract);
                i += ft_strlen(extract);
            }
        }
        else
            result = ft_strjoin(result, &str[i]);
        i++;
    }
    result[i] = '\0';
    return result;
}
// char *expand_ENV(char *str, char **env)
// {
//     int i;
//     int j;
//     char *result;
//     char *check_env;

//     result = ft_calloc(1,1);    
//     i = 0;
//     while(str[i])
//     {
//         if(str[i] == '$' && checkQuoteIndex(str, &str[i]) == 0)
//         {
//             j = i;
//             while(str[i] && (ft_isalpha(str[i]) || str[i] == '_'))
//                 i++;
//             if (i > j)
//                 check_env = ft_substr(str,j,i);
//             if(get_env_var(env,check_env) == -1 && check_env)
//             {
//                 i += ft_strlen(check_env) + 1;
//                 if(check_env)
//                     free(check_env);
//             }
//             else if(get_env_var(env,check_env) != -1 && check_env)
//             {
//                 result = ft_strjoin(result,get_ENV(env, check_env));
//                 i += ft_strlen(get_ENV(env, check_env));
//                 free(check_env);
//             }
//         }
//         result = ft_strjoin(result, &str[i]);
//         i++;
//     }
//     free(str);
//     return(result);
// }

void    final_remove_quotes(t_list *cmds)
{
    int i;
    t_list *tmp;
    
    tmp = cmds;
    while(tmp)
    {
        i = 0;
        while(i < tmp->size_cmd)
        {
            tmp->define[i].content = Expand_quotes(tmp->define[i].content);
            i++;
        }
        tmp = tmp->next;
    }
}

char    *Expand_quotes(char* str)
{
    int i = 0;
    int j = 0;
    int singleQuotes = 0;
    int doubleQuotes = 0;
    int len = ft_strlen(str);
    char* result = (char*)malloc(len + 1);  // Allocate memory for the result string
    if (!result)
        return(NULL);
    while (str[i] != '\0') 
    {
        // Check for single quotes
        if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\') && doubleQuotes % 2 == 0) 
            singleQuotes = (singleQuotes + 1) % 2;
        // Check for double quotes
        else if (str[i] == '"' && (i == 0 || str[i - 1] != '\\') && singleQuotes % 2 == 0) 
            doubleQuotes = (doubleQuotes + 1) % 2;
        // Remove quotes
        else
        {
            if (!(str[i] == '\'' && singleQuotes) && !(str[i] == '"' && doubleQuotes)) 
            {
                result[j] = str[i];
                j++;
            }
        }
        i++;
    }
    free(str);
    result[j] = '\0';  // Add null terminator to the result string
    return result;
}