#include "Minishell.h"

// t_redir *redir_array(t_list *commands)
// {

//     int i;
//     int j = 0;
//     int nb_red;
//     t_list *tmp;
//     tmp = commands;

//     while(tmp)
//     {
//         i = 0;
//         nb_red = 0;
//         while(tmp->cmd[i])
//         {
//             if (ft_strncmp("<<", tmp->cmd[i], 2) == 0 || ft_strncmp(">>", tmp->cmd[i], 2) == 0 ||
//                 ft_strncmp("<", tmp->cmd[i], 1) == 0 || ft_strncmp(">", tmp->cmd[i], 1) == 0)
//                 nb_red++;
//             i++;
//         }
//         if (nb_red > 0)
//         {
//             tmp->redir = malloc(sizeof(t_redir) * nb_red);
//             j = 0;
//             i = 0;
//             while(tmp->cmd)
//             {
//                 if(ft_strncmp("<<", tmp->cmd[i], 2) == 0)
//                 {
//                     tmp->redir->type = HEREDOC;
//                     tmp->redir->red = ft_strdup("<<");
//                     tmp->redir->delimiter = ft_strdup(tmp->cmd[i + 1]);
//                 }
//             }
//         }
//         }


//         tmp = tmp->next;
//     }
