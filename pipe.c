/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 04:54:27 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/21 05:53:02 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	is_builtin(char **args)
{
	if (args)
	{
		if (ft_strcmp(args[0], "echo") == 0)
			return (1);
		else if (ft_strcmp(args[0], "exit") == 0)
			return (1);
		else if (ft_strcmp(args[0], "unset") == 0)
			return (1);
		else if (ft_strcmp(args[0], "cd") == 0)
			return (1);
		else if (ft_strcmp(args[0], "export") == 0)
			return (1);
		else if (ft_strcmp(args[0], "env") == 0)
			return (1);
		else if (ft_strcmp(args[0], "pwd") == 0)
			return (1);
	}
	return (0);
}

int	numberof_cmd(t_list *cmds)
{
	int		nb;
	t_list	*tmp;

	tmp = cmds;
	nb = 0;
	while (tmp)
	{
		nb++;
		tmp = tmp->next;
	}
	return (nb);
}

void	 child_job(int fd[][2], t_general *sa, t_list *tmp, int index,
		int num_cmds)
{
	if (index > 0)
	{
		dup2(fd[index - 1][0], STDIN_FILENO);
		close(fd[index - 1][0]);
		close(fd[index - 1][1]);
	}
	if (index < num_cmds - 1)
	{
		dup2(fd[index][1], STDOUT_FILENO);
		close(fd[index][0]);
		close(fd[index][1]);
	}
	if (is_builtin(tmp->final_cmd) == 1)
	{
		dup_fds(tmp);
		handle_builtins(tmp, sa);
		close_fds(tmp);
	}
	else
	{
		dup_fds(tmp);
		ex_cmd(sa, tmp);
		close_fds(tmp);
	}
	exit(EXIT_SUCCESS);
}

void	parent_job(int fd[][2], int index, t_list *tmp)
{
	close_fds(tmp);
	if (index > 0)
	{
		close(fd[index - 1][0]);
		close(fd[index - 1][1]);
	}
}

int	wait_and_close(int fd[][2], int num_cmds)
{
	int	i;

	i = 0;
	int exit_status = 0;
	while (i < num_cmds - 1)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		wait(&exit_status);
		i++;
	}
	return WEXITSTATUS(exit_status);
}

void	ex_pipe(t_list *cmd, t_general *sa, int num_cmds)
{
	t_list	*tmp;
	int		fd[num_cmds - 1][2];
	pid_t	pid;
	int		index;

	tmp = cmd;
	index = 0;
	while (tmp != NULL)
	{
		if (index < num_cmds - 1)
		{
			if (pipe(fd[index]) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			child_job(fd, sa, tmp, index, num_cmds);
		else
		{
			parent_job(fd, index, tmp);
			tmp = tmp->next;
			index++;
		}
	}
	sa->ex_status = wait_and_close(fd, num_cmds);
}
