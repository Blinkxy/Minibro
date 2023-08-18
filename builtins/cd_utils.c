#include "../Minishell.h"

int remplace_env_var(char **str, int index, char *name)
{
    int len = (ft_strlen(name) + 1);
    str[index] = (char *)malloc(sizeof(char) * len);
    int i = 0;
    while(name[i])
    {
        str[index][i] = name[i];
        i++;
    }
    str[index][i] = '\0'; 
    return(1);
}

int compare_env_var(char *str, char *var)
{
    int i;
    i = 0;
    while(str[i] && str[i] == var[i] && var[i] != '\0' && var[i] != '=')
        i++;
    if((str[i] == '\0' || str[i] == '=') && (var[i] == '\0' || var[i] == '='))
            return(1);
    return(-1);
}

int get_env_var(char **env, char *var)
{
    int i = 0;
    while(env[i])
    {
        if(compare_env_var(env[i], var) == 1)
            return(i);

        i++;        
    }
    return(-1);
}

