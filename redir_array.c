#include "Minishell.h"

int	redir_array_bis(t_list *tmp)
{
	int i;
	int nb_red;

	i = -1;
	nb_red = 0;
	while (++i < tmp->size_cmd)
	{
		if (ft_strncmp("<<", tmp->define[i].content, 2) == 0 || ft_strncmp(">>",
				tmp->define[i].content, 2) == 0 ||
			ft_strncmp("<", tmp->define[i].content, 1) == 0 || ft_strncmp(">",
					tmp->define[i].content, 1) == 0)
			nb_red++;
	}
	return (nb_red);
}

void	redir_heredoc(t_list *tmp, int *i, int *j)
{
	tmp->redir[*j].type = HEREDOC;
	tmp->redir[*j].red = ft_strdup("<<");
	tmp->redir[*j].delimiter = ft_strdup(tmp->define[*i + 1].content);
	(*j)++;
}

void	redir_append(t_list *tmp, int *i, int *j)
{
	tmp->redir[*j].type = APPEND;
	tmp->redir[*j].red = ft_strdup(">>");
	tmp->redir[*j].file = ft_strdup(tmp->define[*i + 1].content);
	(*j)++;
}

void	redir_red_out(t_list *tmp, int *i, int *j)
{
	tmp->redir[*j].type = RED_OUT;
	tmp->redir[*j].red = ft_strdup(">");
	tmp->redir[*j].file = ft_strdup(tmp->define[*i + 1].content);
	(*j)++;
}

void	redir_red_in(t_list *tmp, int *i, int *j)
{
	tmp->redir[*j].type = RED_IN;
	tmp->redir[*j].red = ft_strdup("<");
	tmp->redir[*j].file = ft_strdup(tmp->define[*i + 1].content);
	(*j)++;
}

void	redir_arrayx(t_list *tmp, int *i, int *j)
{
	if (ft_strncmp("<<", tmp->define[*i].content, 2) == 0)
		redir_heredoc(tmp, i, j);

	else if (ft_strncmp(">>", tmp->define[*i].content, 2) == 0)

		redir_append(tmp, i, j);

	else if (ft_strncmp(">", tmp->define[*i].content, 1) == 0)
		redir_red_out(tmp, i, j);

	else if (ft_strncmp("<", tmp->define[*i].content, 1) == 0)
		redir_red_in(tmp, i, j);
}

void	redir_array(t_list *commands)
{
	int i;
	int j;
	int nb_red;
	t_list *tmp;

	tmp = commands;
	while (tmp)
	{
		nb_red = redir_array_bis(tmp);
		if (nb_red > 0)
		{
			tmp->red_nb = nb_red;
			tmp->redir = malloc(sizeof(t_redir) * nb_red);
			i = -1;
			j = 0;
			while (++i < tmp->size_cmd && j < nb_red)
				redir_arrayx(tmp, &i, &j);
		}
		tmp = tmp->next;
	}
}
