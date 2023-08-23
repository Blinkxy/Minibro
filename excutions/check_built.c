#include "../Minishell.h"

void reset_fd(t_list *cmd)
{
    cmd->fd_out = 1;
    cmd->fd_in = 0;
}

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

int handle_builtins(t_list *cmds, t_general *sa)
{
    if(cmds)
    {
        if(ft_strcmp(cmds->final_cmd[0], "cd") == 0)
            sa->ex_status = ft_cd(sa, cmds->final_cmd);
        else if(ft_strcmp(cmds->final_cmd[0], "echo")== 0)
            sa->ex_status = ft_echo(cmds->final_cmd, cmds->fd_out);
        else if(ft_strcmp(cmds->final_cmd[0], "env") == 0)
            sa->ex_status = ft_env(sa, cmds->fd_out);
        else if(ft_strcmp(cmds->final_cmd[0], "export") == 0)
            sa->ex_status = ft_export(sa, cmds->final_cmd, cmds->fd_out);
        else if(ft_strcmp(cmds->final_cmd[0], "pwd") == 0)
            sa->ex_status = ft_pwd(cmds->fd_out);
        else if(ft_strcmp(cmds->final_cmd[0], "unset") == 0)
            sa->ex_status = ft_unset(sa, cmds->final_cmd);
        else if(ft_strcmp(cmds->final_cmd[0], "exit") == 0)
            sa->ex_status = ft_exit(cmds->final_cmd, sa);
        else
            sa->ex_status = execute_external_command(cmds->final_cmd);
    }
    return(1);
}


void handle_redir(t_list *cmd, t_general *sa) 
{
    int nb_r = cmd->red_nb;
    int saved_stdin = dup(0); 
    int saved_stdout = dup(1); 

    while (nb_r > 0) {
        if (cmd->redir->type == RED_IN) {
            cmd->fd_in = open(cmd->redir->file, O_RDONLY);
            dup2(cmd->fd_in, 0);
        } else if (cmd->redir->type == RED_OUT) {
            cmd->fd_out = open(cmd->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
            dup2(cmd->fd_out, 1);
        } else if (cmd->redir->type == APPEND) {
            cmd->fd_out = open(cmd->redir->file, O_CREAT | O_RDWR | O_APPEND, 0644);
            dup2(cmd->fd_out, 1);
        }
        
        nb_r--;
        cmd->redir++;
    }
    handle_builtins(cmd, sa);

    dup2(saved_stdin, 0);
    dup2(saved_stdout, 1);


    close(saved_stdin);
    close(saved_stdout);
}