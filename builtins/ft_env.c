#include "../Minishell.h"

int ft_env(t_general *sa)
{
    int i;

    i = 0;
    if(!sa->env)
        return(1);
    while(sa->env[i])
    {
        ft_putstr_fd(sa->env[i++], 1);
        ft_putstr_fd("\n", 1);
    }
    return(0);
}