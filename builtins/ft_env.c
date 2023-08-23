#include "../Minishell.h"

int ft_env(t_general *sa, int fd)
{
    int i;

    i = 0;
    if(!sa->env)
        return(1);
    while(sa->env[i])
    {
        ft_putstr_fd(sa->env[i++], fd);
        ft_putstr_fd("\n", fd);
    }
    return(0);
}