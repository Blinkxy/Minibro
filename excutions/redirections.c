#include "../Minishell.h"

int handle_redin(t_redir *red)
{
    int fd;
    if(access(red->file, F_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("no such file or directory: ", 2);
        ft_putstr_fd(red->file, 2);
        ft_putstr_fd("\n", 2);
        return(-1);
    }
    if(access(red->file, R_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd("Permission denied: ", 2);
        ft_putstr_fd(red->file, 2);
        ft_putstr_fd("\n", 2);
        return(-1);
    }
    fd = open(red->file, O_RDONLY);
    if(fd > 0)
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    return(fd);
}

int handle_redout(t_redir *red)
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
        return(-1);
    }
    if(fd > 0)
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    return(1);
}


int handle_append(t_redir *red)
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
        return(-1);
    }
    if(fd > 0)
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    return(fd);
}


void make_red(t_list *cmd)
{
    int nb_red;

    nb_red = cmd->red_nb;
    if(cmd->red_nb == 0)
        return;
    if(cmd->fd_in > 2)
        close(cmd->fd_in);
    if(cmd->fd_out > 2)
        close(cmd->fd_out);
    while(nb_red > 0)
    {
        if(cmd->redir->type == RED_IN  )
            cmd->fd_in = handle_redin(cmd->redir);
        else if(cmd->redir->type == RED_OUT)
            cmd->fd_out = handle_redout(cmd->redir);
        else if(cmd->redir->type == APPEND)
            cmd->fd_out = handle_append(cmd->redir);
        nb_red--;
        cmd->redir++;
    }
}

int handle_builtins(t_list *cmds, t_general *sa)
{
    if(cmds)
    {
        make_red(cmds);
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
    }
    return(1);
}

int ex_minishell(t_list *cmd, t_general *sa)
{
    int res;
    int in = dup(0);
    int out = dup(1);

    res = numberOf_cmd(cmd);
    if(is_builtin(cmd->final_cmd) == 0) 
    {
        make_red(cmd);
        if(cmd->fd_in == -1 || cmd->fd_out == -1)
        {
            close(in);
            close(out);
            return(0);
        }
        ex_cmd(sa, cmd->final_cmd);
    }
    else if(res == 1)
        handle_builtins(cmd, sa);

    else
        pipex(cmd, sa);

    dup2(in, 0);
    dup2(out, 1);
    close(in);
    close(out);
    return(1);
}