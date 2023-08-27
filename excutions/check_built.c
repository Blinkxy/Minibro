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


int open_fdheredoc(void) {
    int fd;
    char file[] = "/tmp/.herdoc";
    if (access(file, F_OK) != -1)
        unlink(file);
    fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "open error\n");
        return -1;
    }
    return fd;
}

int handle_heredoc(t_redir *red) {
    int fd;
    char *line;
    fd = open_fdheredoc();
    if (fd == -1)
        return -1;
    
    while (1) {
        line = readline(">");
        if (line == NULL) {
            // Handle readline error or EOF
            break;
        }
        if (strcmp(line, red->delimiter) == 0) {
            free(line);
            close(fd);
            break;
        };
        // Write the line to the file descriptor
        ft_strjoin(line, "\n"); // Add newline character
        //printf("%s\n", line);
        write(fd, line, strlen(line));
        free(line);
    }

    return 0;
}


int check_heredoc(t_redir *red) {
    pid_t id;

    if (red->type == HEREDOC && red->delimiter != NULL) {
        id = fork();
        if (id == -1) {
            // Handle fork error
            return -1;
        } else if (id == 0) {
            // Child process
            handle_heredoc(red); 
            exit(0);
        } else {
            // Parent process
            wait(NULL);
        }
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
        } else if(cmd->redir->type ==  HEREDOC)
            check_heredoc(cmd->redir);
        nb_r--;
        cmd->redir++;
    }
    handle_builtins(cmd, sa);

    dup2(saved_stdin, 0);
    dup2(saved_stdout, 1);

    close(saved_stdin);
    close(saved_stdout);
}