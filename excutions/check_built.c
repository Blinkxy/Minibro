#include "../Minishell.h"

int execute_external_command(char **cmd) 
{
    pid_t pid;

    pid = fork();
    
    if (pid == 0) 
    { // Child process
        execvp(cmd[0], cmd);
        perror("execvp"); // This line will be reached only if execvp fails
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) 
    { // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    } 
    else // Fork failed
        perror("fork");
    return 1; // Return a non-zero value to indicate an error
}

int if_builtin(char **cmd, t_general *sa, int fd)
{
    if(cmd)
    {
        if(ft_strcmp(cmd[0], "cd") == 0)
            sa->ex_status = ft_cd(sa, cmd);
        else if(ft_strcmp(cmd[0], "pwd") == 0)
            sa->ex_status = ft_pwd(fd);
        else if(ft_strcmp(cmd[0], "env") == 0)
            sa->ex_status = ft_env(sa, fd);
        else if(ft_strcmp(cmd[0], "export") == 0)
            sa->ex_status = ft_export(sa, cmd, fd);
        else if(ft_strcmp(cmd[0], "echo") == 0)
            sa->ex_status = ft_echo(cmd, fd);
        else if(ft_strcmp(cmd[0], "unset") == 0)
            sa->ex_status = ft_unset(sa, cmd);
        else if(ft_strcmp(cmd[0], "exit") == 0)
            sa->ex_status = ft_exit(cmd, sa);
        else if(execute_external_command(cmd) != 0)
            sa->ex_status = execute_external_command(cmd);
        else
            return(0);
        
    }
    return(0);
}