#include "../Minishell.h"
int open_fdheredoc(void)
{
    int fd;
    char file[] = "/tmp/.herdoc";
    if(access(file, F_OK) != -1)
        unlink(file);
    fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if(fd == -1)
    {
        ft_putendl_fd("open error\n", 2);
        return(-1);
    }
    return(fd);
}

// int handle_heredoc(t_list *cmd, t_general *sa)
// {
    
// }



int ex_minishell(t_list *cmd, t_general *sa)
{
    int res;

    res = numberOf_cmd(cmd);
    if(res == 1)
    {
        handle_redir(cmd, sa);
    }
    else
        pipex(cmd, sa);
    return(1);
}

