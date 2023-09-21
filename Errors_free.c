/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:54:45 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/18 20:02:45 by mzoheir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Minishell.h"

void	free_define_and_cmd(t_list *cmds)
{
	int		i;
	t_list	*tmp;

	tmp = cmds;
	while (tmp)
	{
		i = 0;
		while (i < tmp->size_cmd)
		{
			if (cmds->define[i].content)
				free(cmds->define[i].content);
			i++;
		}
		if (tmp->define)
			free(tmp->define);
		i = 0;
		if (cmds->cmd)
		{
			while (cmds->cmd[i])
			{
				free(cmds->cmd[i]);
				i++;
			}
			free(cmds->cmd);
		}
		tmp = tmp->next;
	}
}

void	free_struct(t_define *define)
{
	int	i;

	i = 0;
	while (i < define->size_struct)
	{
		if (define[i].content)
			free(define[i].content);
		i++;
	}
	free(define);
}

int	checker_line(char *line)
{
	if (checkquotes(line) == 0)
	{
		printf("Invalid command: quotes not closed\n");
		return (0);
	}
	return (1);
}

int	checker_redir(char *line)
{
	int		i;
	int		j;
	int		len;
	char	*copy;
	int		withinquotes;

	withinquotes = 0;
	copy = ft_strtrim(line, " ");
	len = ft_strlen(copy);
	if (copy[len - 1] == '>' || copy[len - 1] == '<')
	{
		printf("syntax error near unexpected token `newline'\n");
		return (0);
	}
	else if (copy[len - 1] == '|')
	{
		printf("syntax error near unexpected token '|'\n");
		return (0);
	}
	i = 0;
	while (copy[i])
	{
		if (copy[i] == '\'' || copy[i] == '"')
			withinquotes = !withinquotes;
		if (copy[i] == '|' && !withinquotes)
		{
			if ((copy[i + 1] == '|' || copy[i - 1] == '|') && !withinquotes)
			{
				printf("syntax error near unexpected token '|'\n");
				return (0);
			}
		}
		if (copy[i] == '<' && !withinquotes)
		{
			j = i + 1;
			while (copy[j] == ' ' || copy[j] == '\t')
				j++;
			if (copy[j] && (copy[j] == '>' || copy[j] == '|') && (j - 1) > i)
			{
				printf("syntax error near unexpected token `<'\n");
				return (0);
			}
		}
		if (copy[i] == '>' && !withinquotes)
		{
			j = i + 1;
			while (copy[j] == ' ' || copy[j] == '\t')
				j++;
			if (copy[j] && (copy[j] == '<' || copy[j] == '|') && (j - 1) > i)
			{
				printf("syntax error near unexpected token `>'\n");
				return (0);
			}
		}
		if (copy[i] == '>' && !withinquotes)
		{
			if (copy[i + 1] && copy[i + 1] == '>' && !withinquotes)
			{
				j = i + 2;
				while ((copy[j] == ' ' || copy[j] == '\t') && copy[j]
					&& !withinquotes)
					j++;
				if (copy[j] && copy[j] == '>' && !withinquotes)
				{
					printf("syntax error near unexpected token `>'\n");
					return (0);
				}
				else if (copy[j] && copy[j] == '<' && !withinquotes)
				{
					printf("syntax error near unexpected token `<'\n");
					return (0);
				}
			}
		}
		if (copy[i] == '<' && !withinquotes)
		{
			if (copy[i + 1] && copy[i + 1] == '<' && !withinquotes)
			{
				if (copy[i + 2] && (copy[i + 2] == '>' || copy[i + 2] == '<'
						|| copy[i + 2] == '|') && !withinquotes)
					return (0);
			}
		}
		i++;
	}
	free(copy);
	return (1);
}
