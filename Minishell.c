/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 01:17:28 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/26 01:42:23 by mzoheir          ###   ########.fr       */
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

void	init_env_data(t_general *sa, char **envp, int ac, char **av)
{
	int	i;

	i = 0;
	(void)ac;
	(void)av;
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

void	main_bis(t_list *cmds, t_general *sa, t_main *main_struct)
{
	main_struct->st = addnext_pipe(main_struct->s);
	main_struct->str = ft_split(main_struct->st, '\n');
	main_struct->str = remove_pipe_pointers(main_struct->str);
	if (main_struct->str[0])
		cmds = create_node(main_struct->str[0], main_struct->i);
	while (++main_struct->i < count_cmds(main_struct->str))
		add_node_front(cmds, main_struct->str[main_struct->i], main_struct->i);
	cmd_define(cmds);
	final_struct(cmds, sa->env, sa);
	redir_array(cmds);
	final_remove_quotes(cmds);
	final_cmd(cmds);
	default_fds(cmds);
	make_red(cmds, sa);
	ex_test(cmds, sa);
	free(main_struct->str);
	free_all(cmds);
}

int	main(int ac, char **av, char **env)
{
	t_main		main_struct;
	t_list		*cmds;
	t_general	*sa;
	int			stdin;
	int			stdout;

	cmds = NULL;
	sa = malloc(sizeof(t_general));
	sa->cmds = malloc(sizeof(t_list));
	initialize_main(&main_struct, sa);
	memset(sa, 0, sizeof(t_general));
	init_env_data(sa, env, ac, av);
	get_export_env(sa);
	while (1)
	{
		g_sig = 0;
		stdin = dup(STDIN_FILENO);
		stdout = dup(STDOUT_FILENO);
		handle_sig(1);
		main_struct.s = readline("minishell$>");
		if (main_struct.s && main_struct.s[0])
		{
			add_history(main_struct.s);
			if (checker_line(main_struct.s) == 1
				&& checker_redir(main_struct.s) == 1)
				main_bis(cmds, sa, &main_struct);
			else
				handle_sig(3);
		}
	}
	return (0);
}
