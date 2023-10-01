/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 01:17:28 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/30 23:02:46 by mdouzi           ###   ########.fr       */
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

t_list	*parse_commands(char *s)
{
	char	*st;
	char	**stt;
	char	**str;
	int		i;
	t_list	*cmds;

	if (!s || s[0] == '\0')
		return (NULL);
	if (checker_line(s) != 1 || checker_redir(s) != 1)
		return (NULL);
	add_history(s);
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
	cmd_define(cmds);
	final_struct(cmds, sa->env, sa);
	redir_array(cmds);
	final_remove_quotes(cmds);
	final_cmd(cmds);
	default_fds(cmds, sa);
	make_red(cmds, sa);
	signal(SIGINT, restore_pt);
	ex_test(cmds, sa);
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
	init_env_data(sa, env);
	get_export_env(sa);
	s = NULL;
	while (1)
	{
		if (s)
			free(s);
		initialize_signals();
		s = readline("minishell$>");
		if (!s)
			exit(0);
		cmds = parse_commands(s);
		execute_commands(sa, cmds);
		//free_all(cmds);
	}
	return (0);
}
