/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 01:17:28 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/25 23:04:35 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Minishell.h"

int		g_sig = 0;

void	default_fds(t_list *cmds)
{
	t_list	*head;

	head = cmds;
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

void ft_handler(int sig)
{
	(void)sig;
	(void)sig;
	if (g_sig == -1)
	{
		g_sig = -2;
		close (STDIN_FILENO);
	}
	write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int	main(int argc, char **argv, char **env)
{
	int			i;
	char		**str;
	char		*s;
	char		*st;
	t_list		*cmds;
	t_general	*sa;
	int		stdin;
	int 	stdout;

	(void)argc;
	(void)argv;
	cmds = NULL;
	sa = malloc(sizeof(t_general));
	sa->cmds = malloc(sizeof(t_list));
	memset(sa, 0, sizeof(t_general));
	init_env_data(sa, env);
	get_export_env(sa);
	while (1)
	{
		g_sig = 0;
		stdin = dup(STDIN_FILENO);
		stdout = dup(STDOUT_FILENO);
		handle_sig(1);
		s = readline("minishell$>");
		if (s && s[0])
		{
			add_history(s);
			if (checker_line(s) == 1 && checker_redir(s) == 1)
			{
				st = addnext_pipe(s);
				str = ft_split(st, '\n');
				str = remove_pipe_pointers(str);
				i = 0;
				if (str[0])
					cmds = create_node(str[0], i);
				while (++i < count_cmds(str))
					add_node_front(cmds, str[i], i);
				cmd_define(cmds);
				final_struct(cmds, sa->env, sa);
				redir_array(cmds);
				final_remove_quotes(cmds);
				final_cmd(cmds);
				default_fds(cmds);
				make_red(cmds, sa);
				ex_test(cmds, sa);
			}
		}
		else
			handle_sig(3);
	}
	return (0);
}
