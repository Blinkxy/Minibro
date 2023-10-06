/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 01:17:28 by mdouzi            #+#    #+#             */
/*   Updated: 2023/10/06 01:42:04 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int		g_sig = 0;

void	initialize_signals(void)
{
	g_sig = 0;
	signal(SIGINT, restore_pt);
	signal(SIGQUIT, SIG_DFL);
}

t_list	*parse_commands(char *s, t_general *sa)
{
	char	*st;
	char	**stt;
	char	**str;
	int		i;
	t_list	*cmds;

	if (s[0] == '\0')
		return (NULL);
	if (checker_line(s) != 1 || checker_redir(s) != 1)
	{
		sa->ex_status = 258;
		return (NULL);
	}
	st = addnext_pipe(s);
	stt = ft_split(st, '\n');
	str = remove_pipe_pointers(stt);
	if (!str || !str[0])
		return (NULL);
	i = 0;
	cmds = create_node(str[0], i);
	while (++i < count_cmds(str))
		add_node_front(cmds, str[i], i);
	free_double_array(str);
	return (cmds);
}

void	execute_commands(t_general *sa, t_list *cmds)
{
	initialize_signals();
	cmd_define(cmds);
	final_struct(cmds, sa->env, sa);
	update_struct(cmds);
	redir_array(cmds);
	final_remove_quotes(cmds);
	final_cmd(cmds);
	default_fds(cmds, sa);
	make_red(cmds, sa);
	ex_test(cmds, sa);
}

void	init_sa(t_general *sa, char **env)
{
	init_env_data(sa, env);
	get_export_env(sa);
	initialize_signals();
}

int	main(int argc, char **argv, char **env)
{
	char		*s;
	t_general	*sa;
	t_list		*cmds;

	(void)argc;
	(void)argv;
	sa = malloc(sizeof(t_general));
	memset(sa, 0, sizeof(t_general));
	init_sa(sa, env);
	s = NULL;
	while (1)
	{
		g_sig = 0;
		if (s)
			free(s);
		s = readline("minishell$>");
		add_history(s);
		if (!s)
			exit(0);
		cmds = parse_commands(s, sa);
		if (cmds != NULL)
			execute_commands(sa, cmds);
		free_all(cmds);
	}
	return (0);
}
