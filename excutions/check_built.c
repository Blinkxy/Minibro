#include "../Minishell.h"

void reset_fd(t_list *cmd)
{
    cmd->fd_out = 1;
    cmd->fd_in = 0;
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


void handle_heredoc_input(t_list *cmd) {
    
    char *line;
    char *filename = "/tmp/.herdoc"; // Temporary file path

    cmd->has_herdoc = 1;
    cmd->herdoc_content_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (cmd->herdoc_content_fd == -1) 
    {
        ft_putendl_fd("open error\n", 2);
        // Handle error
        return;
    }

    while (1) 
    {
        line = readline(">");
        if (line == NULL || ft_strcmp(line, cmd->redir->delimiter) == 0) {
            // Heredoc is complete
            free(line);
            close(cmd->herdoc_content_fd);
            return;
        }
        // Write the line to the file with a newline character
        write(cmd->herdoc_content_fd, line, strlen(line));
        write(cmd->herdoc_content_fd, "\n", 1);
        free(line);
    }
}

int check_heredoc(t_list *cmd) {
    pid_t id;
    t_list *tmp;

    tmp = cmd;
    if(tmp->red_nb == 0 && tmp->next == NULL)
        return(0);
    if(tmp->red_nb == 0)
        tmp = tmp->next;

    while (tmp) 
    {
        if (tmp->redir->type == HEREDOC ) 
        {
            id = fork();
            if (id == -1) {
                ft_putendl_fd("error fork\n", 2);
                return -1;
            } else if (id == 0) {
                // Child process: Handle heredoc and save it to a file
                handle_heredoc_input(cmd);
                exit(0);
            } else {
                // Parent process
                wait(NULL);
            }
        }
        tmp = tmp->next;
    }
    return 0;
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