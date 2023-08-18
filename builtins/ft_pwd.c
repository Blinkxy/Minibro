#include "../Minishell.h"

int ft_pwd(int fd)
{
    char cur[PATH_MAX];
    if(getcwd(cur, PATH_MAX) == NULL)
        return(1);
    ft_putstr_fd(cur, fd);
    ft_putstr_fd("\n", fd);
    return(0);
}