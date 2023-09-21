/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 03:21:32 by mdouzi            #+#    #+#             */
/*   Updated: 2023/09/20 03:55:01 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Minishell.h"

int	var_export_check(char *env_var)
{
	int	i;

	i = 0;
	if (ft_isalpha(env_var[i]) == 0 && env_var[i] != '_')
		return (1);
	i++;
	while (env_var[i] && env_var[i] != '=')
	{
		if (ft_isalnum(env_var[i]) == 0 && env_var[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

// check number of words for export like var= || var=value
int	check_number_of_wr(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	if (str[0] != c && str[0] != '\0')
		count++;
	while (str[i])
	{
		if (str[i] == c && str[i + 1] != '\0')
			return (2);
		i++;
	}
	return (1);
}

// Function to split the input string for the export command
char	**export_split_var(char *arg)
{
	char	**tmp;
	char	*equal_ptr;

	equal_ptr = ft_strchr(arg, '=');
	if (equal_ptr == NULL)
	{
		tmp = (char **)malloc(sizeof(char *) * 2);
		tmp[0] = ft_substr(arg, 0, ft_strlen(arg));
		tmp[1] = NULL;
	}
	else
	{
		tmp = malloc(sizeof(char *) * (2 + 1));
		tmp[0] = ft_substr(arg, 0, equal_ptr - arg);
		tmp[1] = ft_substr(equal_ptr, 1, ft_strlen(equal_ptr));
		tmp[2] = NULL;
	}
	return (tmp);
}

void	get_export_env(t_general *sa)
{
	int	i;

	i = 0;
	sa->env_export = (char **)malloc(sizeof(char *) * (ft_size(sa->env) + 1));
	while (i < ft_size(sa->env))
	{
		sa->env_export[i] = ft_strdup(sa->env[i]);
		i++;
	}
	sa->env_export[i] = NULL;
}

void	double_swap(int i, int j, t_general *sa)
{
	char	*tmp;

	tmp = ft_strdup(sa->env_export[i]);
	sa->env_export[i] = ft_strdup(sa->env_export[j]);
	sa->env_export[j] = ft_strdup(tmp);
}

// print all the export when i get export cmd with no args
void	solo_export(t_general *sa)
{
	int	i;
	int	j;

	i = 0;
	while (sa->env_export[i])
	{
		j = i + 1;
		while (sa->env_export[j])
		{
			if (ft_strcmp(sa->env_export[i], sa->env_export[j]) > 0)
				double_swap(i, j, sa);
			j++;
		}
		i++;
	}
	i = 0;
	while (sa->env_export[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(sa->env_export[i++], 1);
		ft_putstr_fd("\n", 1);
	}
}

int	double_qchek(char *str)
{
	if (!str)
		return (0);
	else if (str[0] == '"' && str[1] == '"' && str[2] == '\0')
		return (1);
	else if (str[0] == '"' && str[strlen(str) - 1] == '"')
		return (1);
	return (0);
}

char	*copy_name_var(char *name, int len, int equal)
{
	int		i;
	char	*res;

	i = 0;
	res = NULL;
	res = (char *)malloc(sizeof(char) * len);
	while (name[i])
	{
		res[i] = name[i];
		i++;
		if (name[i] == '\0' && equal == 1)
		{
			res[i] = '=';
			while (i < len)
				res[++i] = '"';
		}
	}
	res[i] = '\0';
	return (res);
}

char	*only_name(char *name, char *arg)
{
	int		len;
	char	*res;
	int		i;
	int		equal;

	i = 0;
	len = 0;
	equal = 0;
	if (ft_strchr(arg, '=') != NULL)
	{
		len = ft_strlen(name) + 3;
		equal = 1;
	}
	else
		len = ft_strlen(name);
	res = copy_name_var(name, len, equal);
	return (res);
}
