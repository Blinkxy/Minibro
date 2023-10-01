/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 02:37:22 by mdouzi            #+#    #+#             */
/*   Updated: 2023/10/01 02:39:07 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	handle_redirection(t_list *head, t_general *sa, int i)
{
	if (g_sig == -2)
	{
		close_fds(head);
		return ;
	}
	if (head->redir[i].type == RED_IN)
		head->fd_in = handle_redin(head, head->redir);
	else if (head->redir[i].type == RED_OUT)
		head->fd_out = handle_redout(head, head->redir);
	else if (head->redir[i].type == APPEND)
		head->fd_out = handle_append(head, head->redir);
	else if (head->redir[i].type == HEREDOC)
	{
		head->fd_in = ft_heredoc(head, sa);
		if (head->fd_in == -2)
			return ;
	}
}

int	make_red(t_list *cmd, t_general *sa)
{
	t_list	*head;
	int		nb_red;
	int		i;

	head = cmd;
	nb_red = 0;
	g_sig = -1;
	while (head)
	{
		i = 0;
		nb_red = head->red_nb;
		while (nb_red > 0)
		{
			handle_redirection(head, sa, i);
			i++;
			nb_red--;
		}
		if (head->next == NULL)
			break ;
		head = head->next;
	}
	return (0);
}

void	dup_fds(t_list *cmds)
{
	if (cmds->fd_in != 0)
	{
		if (cmds->fd_in < 0)
			exit(EXIT_FAILURE);
		dup2(cmds->fd_in, STDIN_FILENO);
	}
	if (cmds->fd_out > 1)
	{
		if (cmds->fd_out < 1)
			exit(EXIT_FAILURE);
		dup2(cmds->fd_out, STDOUT_FILENO);
	}
}

void	close_fds(t_list *cmds)
{
	if (cmds->fd_in > 0)
		close(cmds->fd_in);
	if (cmds->fd_out > 1)
		close(cmds->fd_out);
}
