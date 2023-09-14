#include "Minishell.h"

// int     final_size(t_list *cmds)

void	final_cmd(t_list *cmds)
{
	int i;
	int j;
	t_list *tmp;

	tmp = cmds;
	while (tmp)
	{
		tmp->final_cmd = (char **)malloc(sizeof(char *) * (tmp->size_cmd
					- (tmp->red_nb * 2)) + 1);
		if (!tmp->final_cmd)
			return ;
		i = 0;
		j = 0;
		while (i < tmp->size_cmd)
		{
			if (tmp->define[i].state == WORD)
			{
				tmp->final_cmd[j] = ft_strdup(tmp->define[i].content);
				j++;
			}
			i++;
		}
		tmp->final_cmd[j] = NULL;
		tmp = tmp->next;
	}
}


