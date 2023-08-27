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

void pipex(t_list *cmd, t_general *sa) {
    t_list *tmp;
    int index;
    int fd[numberOf_cmd(cmd) - 1][2];
    pid_t id[numberOf_cmd(cmd)];

    index = 0;
    int i = 0;
    tmp = cmd;

    while (index < numberOf_cmd(cmd)) {
        if (index < numberOf_cmd(cmd) - 1) {
            pipe(fd[index]);
        }

        id[index] = fork();

        if (id[index] == -1) {
            ft_putendl_fd("error", 2);
            exit(EXIT_FAILURE);
        } else if (id[index] == 0) {
            if (index == 0)
                dup2(fd[index][1], STDOUT_FILENO);
            else 
            {
                dup2(fd[index - 1][0], STDIN_FILENO);
                dup2(fd[index][1], STDOUT_FILENO);
            }

            i = 0;
            while (i < numberOf_cmd(cmd) - 1) {
                close(fd[i][0]);
                close(fd[i][1]);
                i++;
            }

            if (is_builtin(tmp->final_cmd) == 1) {
                handle_builtins(tmp, sa);
            } else {
                execute_external_command(tmp->final_cmd);
            }
            
            exit(EXIT_SUCCESS);
        }

        if (index > 0) {
            i = 0;
            while (i < numberOf_cmd(cmd) - 1) {
                close(fd[i][0]);
                close(fd[i][1]);
                i++;
            }
        }

        tmp = tmp->next;
        index++;
    }

    i = 0;
    while (i < numberOf_cmd(cmd) - 1) {
        close(fd[i][0]);
        close(fd[i][1]);
        i++;
    }

    i = 0;
    while (i < numberOf_cmd(cmd)) {
        waitpid(id[i], NULL, 0);
        i++;
    }
}
