/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 05:48:21 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/21 07:07:47 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	handle_builtins(t_list *cmds, t_general *sa)
{
	if (cmds)
	{
		if (ft_strcmp(cmds->final_cmd[0], "cd") == 0)
			sa->ex_status = ft_cd(sa, cmds->final_cmd);
		else if (ft_strcmp(cmds->final_cmd[0], "echo") == 0)
			sa->ex_status = ft_echo(cmds->final_cmd);
		else if (ft_strcmp(cmds->final_cmd[0], "env") == 0)
			sa->ex_status= ft_env(sa);
		else if (ft_strcmp(cmds->final_cmd[0], "export") == 0)
			sa->ex_status = ft_export(sa, cmds->final_cmd);
		else if (ft_strcmp(cmds->final_cmd[0], "pwd") == 0)
			sa->ex_status = ft_pwd();
		else if (ft_strcmp(cmds->final_cmd[0], "unset") == 0)
			sa->ex_status = ft_unset(sa, cmds->final_cmd);
		else if (ft_strcmp(cmds->final_cmd[0], "exit") == 0)
			sa->ex_status = ft_exit(cmds->final_cmd, sa);
	}
	return (0);
}

void	reset_fds(int in, int out)
{
	dup2(in, STDIN_FILENO);
	close(in);
	dup2(out, STDOUT_FILENO);
	close(out);
}

int	ex_builtins(t_list *cmd, t_general *sa)
{
	int	in;
	int	out;

	in = dup(0);
	out = dup(1);
	if (cmd->fd_in > 0)
		dup2(cmd->fd_in, STDIN_FILENO);
	if (cmd->fd_out > 1)
		dup2(cmd->fd_out, STDOUT_FILENO);
	handle_builtins(cmd, sa);
	close(cmd->fd_in);
	close(cmd->fd_out);
	reset_fds(in, out);
	return (0);
}

void	ex_test(t_list *cmd, t_general *sa)
{
	t_list	*head;
	int		num_cmds;

	if (g_sig == -2)
		return ;
	head = cmd;
	num_cmds = numberof_cmd(cmd);
	if (cmd && !cmd->next && is_builtin(cmd->final_cmd) == 1 && cmd->final_cmd)
		ex_builtins(cmd, sa);
	else
		ex_pipe(cmd, sa, num_cmds);
	while (head)
	{
		close_fds(head);
		if (head->next == NULL)
			break ;
		head = head->next;
	}
}
