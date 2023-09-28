#include "../Minishell.h"

int	handle_redin(t_list *cmd, t_redir *red)
{
	int	fd;

	if (access(red->file, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("no such file or directory: ", 2);
		ft_putstr_fd(red->file, 2);
		ft_putstr_fd("\n", 2);
		return (-3);
	}
	if (access(red->file, R_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("Permission denied: ", 2);
		ft_putstr_fd(red->file, 2);
		ft_putstr_fd("\n", 2);
		return (-2);
	}
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	fd = open(red->file, O_RDONLY);
	return (fd);
}

int	handle_redout(t_list *cmd, t_redir *red)
{
	int	fd;

	fd = open(red->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (access(red->file, W_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("Permission denied: ", 2);
		ft_putstr_fd(red->file, 2);
		ft_putstr_fd("\n", 2);
		close(fd);
		return (-2);
	}
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	return (fd);
}

int	handle_append(t_list *cmd, t_redir *red)
{
	int	fd;

	fd = open(red->file, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (access(red->file, W_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("Permission denied: ", 2);
		ft_putstr_fd(red->file, 2);
		ft_putstr_fd("\n", 2);
		close(fd);
		return (-3);
	}
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	return (fd);
}

int	hrdc_expand(char *delimiter)
{
	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
		return (0);
	return (1);
}

int	ft_heredoc(t_list *cmds, t_general *sa)
{
	char	*del;
	int		pipefd[2];
	int		child_pid;
	char	*line;
	char	*tmp;
	int		status;

	pipe(pipefd);
	del = ft_strdup(cmds->redir->delimiter);
	del = expand_quotes(del);
	// Register the signal handler
	if ((child_pid = fork()) == -1)
	{
		free(del);
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		close(pipefd[0]); // Close the read end of the pipe
		signal(SIGINT, SIG_DFL);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				free(del);
				close(pipefd[1]);
				exit(EXIT_SUCCESS);
			}
			if (ft_strcmp(line, del) == 0)
			{
				free(del);
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
				g_sig = -2;
				return (-2);
			}
		}
	}
	free(del);
	return (pipefd[0]);
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
			if (g_sig == -2)
			{
				close_fds(head);
				return (-2);
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
					return (0);
			}
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
