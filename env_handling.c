/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdouzi < mdouzi@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 21:30:26 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/21 07:04:07 by mdouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

int	count_cmds(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*extract_env(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	return (ft_substr(str, i, ft_strlen(str) - i));
}

char	*expand_env(char *str, char **env)
{
	int		i;
	int		j;
	char	*result;
	char	*check_env;
	char	*extract;

	result = ft_calloc(1, 1);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && check_dollar(str, i) == 0)
		{
			if (str[i + 1] == '?')
			{
			// 	printf("%d\n", gb_ex_st);
				i += 2;
			}
			else
			{
				j = i + 1;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				if (j > i + 1)
				{
					check_env = ft_substr(str, i + 1, j - i - 1);
					if (get_env_var(env, check_env) == -1 && check_env)
						i += ft_strlen(check_env);
					else if (get_env_var(env, check_env) != -1 && check_env)
					{
						extract = extract_env(env[get_env_var(env,
																check_env)]);
						result = ft_strjoin(result, extract);
						i += ft_strlen(check_env);
						free(extract);
						free(check_env);
					}
				}
			}
		}
		else
			result = concatenate_char(result, str[i]);
		i++;
	}
	free(str);
	return (result);
}
