/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_bis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 22:34:03 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/30 22:54:54 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	default_fds(t_list *cmds, t_general *sa)
{
	t_list	*head;

	head = cmds;
	sa->num_cmds = 0;
	sa->index = 0;
	sa->pid = 1;
	while (head)
	{
		head->fd[0] = -1;
		head->fd[1] = -1;
		head = head->next;
	}
}

void	init_env_data(t_general *sa, char **envp)
{
	int	i;

	i = 0;
	sa->env = malloc(sizeof(char *) * (ft_size(envp) + 1));
	while (envp[i])
	{
		sa->env[i] = ft_strdup(envp[i]);
		i++;
	}
	sa->env[i] = NULL;
}

void	ft_handler(int sig)
{
	(void)sig;
	(void)sig;
	if (g_sig == -1)
	{
		g_sig = -2;
		close(STDIN_FILENO);
	}
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	restore_pt(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	killo(int sig)
{
	(void)sig;
	printf("exit\n");
	exit(0);
}
