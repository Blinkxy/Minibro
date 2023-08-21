#include "../Minishell.h"

char **export_update(char **env, char *var) 
{
    int i;
    char **new_export;
    int j;
    
    i = 0;
    j = 0;
    new_export = NULL;
    while (env[i] != NULL)
        i++;
    new_export = (char **)malloc(sizeof(char *) * (i + 2));
    while(j < i)
    {
        new_export[j] = ft_strdup(env[j]);
        j++;
        if(env[j] == NULL)
            free_tab(env);
    }
    new_export[i] = ft_strdup(var);
    new_export[i + 1] = NULL;
    return new_export;
}


char *with_value(char **new_var)
{
    char *res = NULL;
        size_t len_res = ft_strlen(new_var[0]) + ft_strlen(new_var[1]) + 4; // +4 for the equal sign, 2 double quotes, and null terminator
        res = (char *)malloc(len_res + 1); // +1 for the null terminator
        if (!res)
            return NULL;

        ft_strlcpy(res, new_var[0], len_res + 1);
        ft_strlcat(res, "=", len_res + 1);
        ft_strlcat(res, "\"", len_res + 1);
        ft_strlcat(res, new_var[1], len_res + 1);
        ft_strlcat(res, "\"", len_res + 1);
    return res;
}


int check_or_update(char **new_var,char *var, t_general *sa)
{
    int index;

    index = -1;
    index = get_env_var(sa->env_export, new_var[0]);
    printf("index == %d\n", index);
    if(index != -1 && ft_strchr(var, '=') != NULL)
    {
        free(sa->env_export[index]);
        free(sa->env[index]);
        sa->env[index]= ft_strdup(var);
        sa->env_export[index] = ft_strdup(var);
    }
    else if(index != -1 && ft_strchr(var, '=') == NULL)
        return(0);
    else
    {
        sa->env_export = export_update(sa->env_export, var);
        sa->env = export_update(sa->env, var);
    }
    return(1);
}

int ft_export(t_general *sa, char **cmd, int fd)
{
    int i;
    int res;
    char **new_var;
    char *var;

    res = 0;
    i = 1;
    var =NULL;
    new_var = NULL;
    if(cmd[1] == NULL)
    {
        solo_export(sa, fd);
        res = 1;
    }
    while(cmd[i])
    {
        if(var_export_check(cmd[i]) == 1)
        {
            ft_putstr_fd("export: `", 2);
            ft_putstr_fd(cmd[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            res = 1;
        }
        else
        {
            new_var = export_split_var(cmd[i]);
            if(new_var[1] == NULL)
            {
                var = only_name(new_var[0], cmd[i]);
                check_or_update(new_var, var, sa);  
            }
            else
            {
                var = with_value(new_var);
               check_or_update(new_var, var, sa);
            }
             free(new_var);
             free(var);
        }
        i++;
    }
    return(res);
}



