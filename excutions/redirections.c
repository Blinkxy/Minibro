#include "../Minishell.h"

int handle_redin(t_list *cmd, t_redir *red)
{
    int fd;
    if(access(red->file, F_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("no such file or directory: ", 2);
        ft_putstr_fd(red->file, 2);
        ft_putstr_fd("\n", 2);
        return(-3);
    }
    if(access(red->file, R_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("Permission denied: ", 2);
        ft_putstr_fd(red->file, 2);
        ft_putstr_fd("\n", 2);
        return(-2);
    }
    if(cmd->fd_in > 2)
        close(cmd->fd_in);
    fd = open(red->file, O_RDONLY);

    return(fd);
}

int handle_redout(t_list *cmd, t_redir *red)
{
    int fd;
    fd = open(red->file,  O_CREAT | O_RDWR | O_TRUNC, 0644);
    if(access(red->file, W_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("Permission denied: ", 2);
        ft_putstr_fd(red->file, 2);
        ft_putstr_fd("\n", 2);
        close(fd);
        return(-2);
    }
    if(cmd->fd_out > 2)
        close(cmd->fd_out);
    return(fd);
}


int handle_append(t_list *cmd, t_redir *red)
{
    int fd;
    fd = open(red->file, O_CREAT | O_RDWR | O_APPEND, 0644);
    if(access(red->file, W_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("Permission denied: ", 2);
        ft_putstr_fd(red->file, 2);
        ft_putstr_fd("\n", 2);
        close(fd);
        return(-3);
    }
    if(cmd->fd_out > 2)
        close(cmd->fd_out);
    return(fd);
}


int ft_heredoc(t_list *cmds, t_general *sa)
{
    char *line;
    int pipefd[2];
    pipe(pipefd);
    (void)sa;
    while(1)
    {
        line = readline("> ");
        if(ft_strcmp(line , cmds->redir->delimiter) == 0)
        {
            close(pipefd[1]);
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    return(pipefd[0]);
}


int make_red(t_list *cmd, t_general *sa)
{
    t_list *head;
    int nb_red;

    head = cmd;
    nb_red = 0;
    while(head)
    {
        nb_red = head->red_nb;
        while(nb_red > 0)
        {
            if(head->redir->type == RED_IN)
                 head->fd_in = handle_redin(head, head->redir);
             else if(head->redir->type == RED_OUT)
                head->fd_out = handle_redout(head, head->redir);
            else if(head->redir->type == APPEND)
                head->fd_out = handle_append(head, head->redir);
            else if(head->redir->type == HEREDOC)
                head->fd_in = ft_heredoc(head, sa);
            head->redir++;
            nb_red--;
        }
        if(head->next == NULL)
            break;
        head = head->next;
    }
    return(0);
}


void dup_fds(t_list *cmds)
{
    if(cmds->fd_in != 0)
    {
        if(cmds->fd_in < 0)
            exit(EXIT_FAILURE);
        dup2(cmds->fd_in, STDIN_FILENO);
    }
    if(cmds->fd_out > 1)
    {
        if(cmds->fd_out < 1)
            exit(EXIT_FAILURE);
        dup2(cmds->fd_out, STDOUT_FILENO);
    }
}

void close_fds(t_list *cmds)
{
    if(cmds->fd_in > 0)
        close(cmds->fd_in);
    if(cmds->fd_out > 1)
        close(cmds->fd_out);
}
