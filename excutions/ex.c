#include "../Minishell.h"

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