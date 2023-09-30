/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:43:49 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/30 00:59:12 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	hrdc_expand(char *delimiter)
{
	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
		return (0);
	return (1);
}

void child_heredoc(char *del, int pipefd[2], t_list *cmds, t_general *sa)
{
    char *line;
    char *tmp;
    
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

int	ft_heredoc(t_list *cmds, t_general *sa)
{
	char	*del;
	int		pipefd[2];
	int		child_pid;
	int		status;

	pipe(pipefd);
	del = ft_strdup(cmds->redir->delimiter);
	del = expand_quotes(del);
	if ((child_pid = fork()) == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
        child_heredoc(del, pipefd, cmds, sa);
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