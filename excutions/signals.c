/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:00:49 by mdouzi            #+#    #+#             */
/*   Updated: 2023/10/05 04:49:10 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	restore_pt(int sig)
{
	if (g_sig == 1)
		kill (0, SIGINT);
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	ctrl_c(int sig)
{
	g_sig = -2;
	write(1, "\n", 1);
	(void)sig;
}

void	back_slash(int sig)
{
	printf("Quit (core dumped)\n");
	(void)sig;
}

void	sig_kill(int sig)
{
	(void)sig;
	printf("exit\n");
	exit(0);
}

void	handle_sig(int mode)
{
	if (mode == 1)
	{
		signal(SIGINT, restore_pt);
		signal(SIGQUIT, sig_kill);
	}
	if (mode == 2)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, back_slash);
	}
	if (mode == 3)
	{
		printf("exit\n");
		exit(0);
	}
	if (mode == 4)
	{
		g_sig = -2;
		signal(SIGINT, SIG_DFL);
	}
}
