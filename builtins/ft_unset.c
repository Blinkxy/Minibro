#include "../Minishell.h"

int ft_unset(t_general *sa, char **cmd)
{
    int i = 1;
    int res = 0;
    int index = -1;

    while (cmd[i])
    {
        if (var_export_check(cmd[i]) == 1 || ft_strchr(cmd[i], '=') != NULL)
        {
            ft_putstr_fd("unset:  ", 2);
            ft_putstr_fd(cmd[i], 2);
            ft_putstr_fd(" : not a valid identifier\n", 2);
            res = 1;
        }
        else
        {
            index = get_env_var(sa->env_export, cmd[i]);
            if (index != -1)
            {
                // Free the memory occupied by the environment variable
                free(sa->env_export[index]);
                // Move the pointers of the array to overwrite the removed variable
                for (int j = index; sa->env_export[j]; j++)
                {
                    sa->env_export[j] = sa->env_export[j + 1];
                }
                res = 0; // Variable successfully unset
            }
        }
        i++;
    }

    return res;
}
