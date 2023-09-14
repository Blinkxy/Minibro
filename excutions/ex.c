#include "../Minishell.h"

char *make_path(char *path)
{
    int i;
    int j;
    char *res;
    i = 0;
    j = 0;
    while(path[i] && path[i] != '=')
        i++;
    if(path[i] == '=')
        i++;
    res = (char *)malloc(sizeof(char) * (ft_strlen(path) - i));
    while(path[i])
    {
        res[j] = path[i];
        i++;
        j++;
    }
    res[j] = '\0';
    return(res);
}

char *get_path(char **env, char *cmd)
{
    char *path; 
    char **splited;
    char *fc;
    char *res;

    res = NULL;
    path = make_path(env[get_env_var(env, "PATH")]);
    splited = ft_split(path, ':');
    fc = ft_strjoin("/", cmd);

    int i = 0;
    while(splited[i])
    {
        splited[i] = ft_strjoin(splited[i], fc);
        if(access(splited[i], F_OK) == 0)
        {
            res = ft_strdup(splited[i]);
            free(splited);
            //free(path);
            break;
        }
        i++;
    }
    if(res == NULL)
    {
        ft_putstr_fd("minishell : ", 2);
        ft_putstr_fd("command not found: ", 2);
        ft_putstr_fd(cmd, 2);
        ft_putstr_fd("\n", 2);
        exit(1);
    }
    return(res);
}


void ex_cmd(t_general *sa, char **cmd)
{
    pid_t id;
    
    id = fork();
    if(id < 0)
    {
        ft_putstr_fd("fork\n", 2);
        return;
    }
    if(id == 0)
    {
        if(cmd[0][0] == '.' || cmd[0][0] == '/')
        {
            if(access(cmd[0], F_OK) != 0)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd("command not found: ", 2);
                ft_putstr_fd(cmd[0], 2);
                ft_putstr_fd("\n", 2);
                exit(1);
            }
            else
            {
                execve(cmd[0], cmd, sa->env);
                exit(0);
            }
        }
        else 
        {
            cmd[0] = ft_strdup(get_path(sa->env, cmd[0]));
            execve(cmd[0], cmd, sa->env);
            printf("cmd\n");
            exit(0);
        }
    }
    else
        wait(NULL);
    
}

