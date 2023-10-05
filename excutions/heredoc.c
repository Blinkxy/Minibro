/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:43:49 by mdouzi            #+#    #+#             */
/*   Updated: 2023/10/05 05:33:18 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

void	handle_heredoc_line(char *line, int pipefd[2], t_general *sa,
		t_list *cmds)
{
	if (ft_strcmp(line, sa->del) == 0)
	{
		close(pipefd[1]);
		free(line);
		exit(EXIT_SUCCESS);
	}
	else if (hrdc_expand(cmds->redir->delimiter) == 1)
		write_exp(sa, line, pipefd);
	else
	{
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	write(pipefd[1], "\n", 1);
}

void	sig_hero(int sig)
{
	exit(1);
	(void)sig;
}

void	child_heredoc(int pipefd[2], t_list *cmds, t_general *sa)
{
	char	*line;

	close(pipefd[0]);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipefd[1]);
			exit(EXIT_SUCCESS);
		}
		handle_heredoc_line(line, pipefd, sa, cmds);
	}
}

int	ft_heredoc(t_list *cmds, t_general *sa)
{
	int	pipefd[2];
	int	child_pid;
	int	status;

	pipe(pipefd);
	sa->del = ft_strdup(cmds->redir->delimiter);
	sa->del = expand_quotes(sa->del);
	child_pid = fork();
	if (child_pid == -1)
		error_fork();
	if (child_pid == 0)
		child_heredoc(pipefd, cmds, sa);
	else
	{
		close(pipefd[1]);
		if (waitpid(child_pid, &status, 0) == child_pid && WIFSIGNALED(status))
		{
			free(sa->del);
			close(pipefd[0]);
			return (-2);
		}
	}
	free(sa->del);
	return (pipefd[0]);
}
