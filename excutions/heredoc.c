#include "../Minishell.h"

int	handle_herdoc(t_list *cmds, t_general *sa)
{
	pid_t	id;
	int		wait_st;
	int		fd[2];

	pipe(fd);
	id = fork();
	if (id == 0)
		heredoc(cmds, sa, fd);
	waitpid(id, &wait_st, 0);
	if (WIFEXITED(wait_st))
	{
		gb_ex_st = WEXITSTATUS(wait_st);
		close(fd[1]);
	}
	return (0);
}

void	heredoc(t_list *cmds, t_general *sa, int fd[2])
{
	char	*line;

	(void)sa;
	close(fd[0]);
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, cmds->redir->delimiter) == 0)
		{
			close(fd[1]);
			free(line);
			cmds->fd_in = fd[0];
			return ;
		}
		write(fd[1], line, strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}

int	check_herdoc(t_list *cmd, t_general *sa)
{
	while (cmd)
	{
		if (cmd->redir->type == HEREDOC && cmd->redir->delimiter)
			handle_herdoc(cmd, sa);
		cmd = cmd->next;
	}
	return (0);
}