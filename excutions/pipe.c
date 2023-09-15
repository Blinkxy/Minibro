#include "../Minishell.h"

int is_builtin(char **args)
{
    if(args)
    {
        if(ft_strcmp(args[0], "echo") == 0)
            return(1);
        else if(ft_strcmp(args[0], "exit") == 0)
            return(1);
        else if(ft_strcmp(args[0], "unset") == 0)
            return(1);
        else if(ft_strcmp(args[0], "cd") == 0)
            return(1);
        else if(ft_strcmp(args[0], "export") == 0)
            return(1);
        else if(ft_strcmp(args[0], "env") == 0)
            return(1);
        else if(ft_strcmp(args[0], "pwd") == 0)
            return(1);        
    }
        return(0);
}
 
int numberOf_cmd(t_list *cmds)
{
    int nb;
    t_list *tmp;

    tmp = cmds;
    nb = 0;
    while(tmp)
    {
        nb++;
        tmp = tmp->next;
    }
    return(nb);
}


#include <sys/wait.h>

void ex_pipe(t_list *cmd, t_general *sa) {
    t_list *tmp = cmd;
    int num_cmds = numberOf_cmd(cmd);

    int fd[num_cmds - 1][2];
    pid_t pid;

    int index = 0;
    while (tmp != NULL) 
    {
        if (index < num_cmds - 1) 
        {
            if (pipe(fd[index]) == -1) 
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == -1) 
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) 
        {
            // Child process
            if (index > 0) 
            {
                // Redirect input from the previous pipe
                dup2(fd[index - 1][0], STDIN_FILENO);
                close(fd[index - 1][0]);
                close(fd[index - 1][1]);
            }

            if (index < num_cmds - 1) 
            {
                // Redirect output to the next pipe
                dup2(fd[index][1], STDOUT_FILENO);
                close(fd[index][0]);
                close(fd[index][1]);
            }

            // Handle the command execution (similar to your ex_child function)
            if (is_builtin(tmp->final_cmd) == 1) {
                dup_fds(tmp);
                handle_builtins(tmp, sa);
                close_fds(tmp);
            } else {
                dup_fds(tmp);
                ex_cmd(sa, tmp);
                close_fds(tmp);
            }

            exit(EXIT_SUCCESS);
        }
         else 
        {
            // Parent process
            close_fds(tmp);
            if (index > 0) {
                close(fd[index - 1][0]);
                close(fd[index - 1][1]);
            }

            tmp = tmp->next;
        }

        index++;
    }

    // Close any remaining pipes in the parent process
    int i = 0;
    while (i < num_cmds - 1) {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }

    // Wait for all child processes to finish
    i = 0;
    while (i < num_cmds) {
        wait(NULL);
        i++;
    }
}
