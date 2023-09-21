/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 03:56:08 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/21 06:40:35 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	update_pwd(t_general *sa)
{
	char	pwd[PATH_MAX];
	char	*res;
	int		index;

	if (getcwd(pwd, PATH_MAX) == NULL)
		return (1);
	res = env_join("PWD=", pwd);
	index = get_env_var(sa->env, "PWD");
	if (index == -1)
		return (1);
	remplace_env_var(sa->env, index, res);
	return (0);
}

int	ft_cd(t_general *sa, char **cmd)
{
	int	res;

	res = 0;
	if (!cmd[1])
	{
		res = cd_home(sa);
		if(res != 1)
			update_pwd(sa);
	}
	else
	{
		update_oldpwd(sa);
		res = chdir(cmd[1]);
		if (res != 0)
		{
			ft_putstr_fd("cd:", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putstr_fd(" no such file or directory \n", 2);
		}
		update_pwd(sa);
	}
	return (res);
}

int	cd_home(t_general *sa)
{
	char	*home_path;
	int		index;
	int		res;

	res = 0;
	home_path = NULL;
	index = get_env_var(sa->env, "HOME");
	if (index == -1)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	home_path = get_path_env(sa->env[index]);
	update_oldpwd(sa);
	res = chdir(home_path);
	return (res);
}

int	update_oldpwd(t_general *sa)
{
	char	cur[PATH_MAX];
	char	*oldpwd;
	int		index;

	if (getcwd(cur, PATH_MAX) == NULL)
		return (1);
	oldpwd = env_join("OLDPWD=", cur);
	index = get_env_var(sa->env, "OLDPWD");
	if (!index)
		return (1);
	remplace_env_var(sa->env, index, oldpwd);
	return (0);
}

char	*get_path_env(char *env)
{
	int		i;
	int		j;
	char	*res;
	int		path_length;

	i = 0;
	j = 0;
	res = NULL;
	while (env[i] != '=')
		i++;
	i++;
	path_length = strlen(env) - i;
	res = (char *)malloc(sizeof(char) * (path_length + 1));
	while (env[i])
	{
		res[j] = env[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*env_join(char *s1, char *s2)
{
	int	len;
	int	i;
	int	j;
	char	*res;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = (char *)malloc(sizeof(char) * len + 1);
	while (s1[i])
	{
		res[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		res[j] = s2[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
}