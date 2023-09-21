/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 04:11:13 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/20 04:11:16 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

char	*make_path(char *path)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	while (path[i] && path[i] != '=')
		i++;
	if (path[i] == '=')
		i++;
	res = (char *)malloc(sizeof(char) * (ft_strlen(path) - i));
	while (path[i])
	{
		res[j] = path[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*get_path(char **env, char *cmd)
{
	char	*path;
	char	**splited;
	char	*fc;
	char	*res;
	int		i;

	res = NULL;
	path = make_path(env[get_env_var(env, "PATH")]);
	splited = ft_split(path, ':');
	fc = ft_strjoin("/", cmd);
	i = 0;
	while (splited[i])
	{
		splited[i] = ft_strjoin(splited[i], fc);
		if (access(splited[i], F_OK) == 0)
		{
			res = ft_strdup(splited[i]);
			free(splited);
			//free(path);
			break ;
		}
		i++;
	}
	if (res == NULL)
	{
		ft_putstr_fd("minishell : ", 2);
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd("\n", 2);
		exit(1);
	}
	return (res);
}

void	ex_cmd(t_general *sa, t_list *cmd)
{
	if (cmd->final_cmd[0][0] == '.' || cmd->final_cmd[0][0] == '/')
	{
		if (access(cmd->final_cmd[0], F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(cmd->final_cmd[0], 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
		else
		{
			//dup_fds(cmd);
			execve(cmd->final_cmd[0], cmd->final_cmd, sa->env);
			//close_fds(cmd);
		}
	}
	else
	{
		cmd->final_cmd[0] = ft_strdup(get_path(sa->env, cmd->final_cmd[0]));
		//dup_fds(cmd);
		execve(cmd->final_cmd[0], cmd->final_cmd, sa->env);
		//close_fds(cmd);
	}
}
