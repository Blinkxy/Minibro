/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:27:38 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/26 16:29:23 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	ft_heredoc(t_list *cmds, t_general *sa)
{
	char	*del;
	int		pipefd[2];
	int		child_pid;
	int		status;
	char	*line;
	char	*tmp;

	pipe(pipefd);
	del = ft_strdup(cmds->redir->delimiter);
	del = expand_quotes(del);
	if ((child_pid = fork()) == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
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
			if (ft_strcmp(line, del) == 0)
			{
				close(pipefd[1]);
				free(line);
				exit(EXIT_SUCCESS);
			}
			else if (hrdc_expand(cmds->redir->delimiter) == 1)
			{
				tmp = ft_strdup(del);
				tmp = expand_env(line, sa->env, sa);
				write(pipefd[1], tmp, ft_strlen(tmp));
				free(tmp);
			}
			else
			{
				write(pipefd[1], line, ft_strlen(line));
				free(line);
			}
			write(pipefd[1], "\n", 1);
		}
	}
	else
	{
		close(pipefd[1]);
		if (waitpid(child_pid, &status, 0) == child_pid)
		{
			if (WIFSIGNALED(status))
			{
				close(pipefd[0]);
				return (-2);
			}
		}
	}
	free(del);
	return (pipefd[0]);
}
