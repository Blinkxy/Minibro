#include "../Minishell.h"

int ft_cd(t_general *sa, char **cmd)
{
    int res;

    res = 0;
    if(!cmd[1])
       res = cd_home(sa);
    else
    {
        update_oldpwd(sa);
        res = chdir(cmd[1]);
        if(res != 0)
        {
            ft_putstr_fd("cd:", 2);
            ft_putstr_fd(cmd[1], 2);
            ft_putstr_fd(" no such file or directory \n", 2);

        }
    }
    return(res);
}

int cd_home(t_general *sa)
{
    char *home_path;
    int index;
    int res;
    res = 0;
    home_path = NULL;
    index = get_env_var(sa->env, "HOME");
    if(!index)
    {
        ft_putstr_fd("cd: HOME not set\n", 2);
        return(1);
    }
    home_path = get_path_env(sa->env[index]);
    update_oldpwd(sa);
    res =chdir(home_path);
    return(res);
}


int update_oldpwd(t_general *sa)
{
    char cur[PATH_MAX];
    char *OLDPWD;
    int index;
    if(getcwd(cur, PATH_MAX) == NULL)
        return(1);
    OLDPWD = env_join("OLDPWD=", cur);
    index = get_env_var(sa->env, "OLDPWD");
    if(!(index = get_env_var(sa->env, "OLDPWD")))
        return(1);
    remplace_env_var(sa->env, index, OLDPWD);
    return(0);
}

char *get_path_env(char *env) 
{
    int i = 0;
    int j = 0;
    char *res = NULL;

    while (env[i] != '=')
        i++;
    i++;
    int path_length = strlen(env) - i;

    res = (char *)malloc(sizeof(char) * (path_length + 1));
    while (env[i]) {
        res[j] = env[i];
        i++;
        j++;
    }
    res[j] = '\0';

    return res;
}


char *env_join(char *s1, char *s2)
{
    int len; 
    char * res;
    int i;
    int j;

    i = 0;
    j = 0;
    len = ft_strlen(s1) + ft_strlen(s2) + 1;
    res = (char *)malloc(sizeof(char) * len + 1);
    
    while(s1[i])
    {
        res[j] = s1[i];
        i++;
        j++;
    }
    i = 0;
    while(s2[i])
    {
        res[j] = s2[i];
        i++;
        j++;  
    }
    res[j] = '\0';
    return(res);
}