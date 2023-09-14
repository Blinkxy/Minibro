#include "Minishell.h"

int	skip_quote(char *str, int i)
{
	if (str[i] == '\"')
	{
		i++;
		while (str[i] != '\"')
			i++;
	}
	else if (str[i] == '\'')
	{
		i++;
		while (str[i] != '\'')
			i++;
	}
	return (i);
}
void	count_word_bis(char *str, t_count *count_words, int *i)
{
	if (str[*i] == '\'')
	{
		if (!count_words->inDoubleQuotes)
			count_words->inSingleQuotes = !count_words->inSingleQuotes;
		else
			count_words->wordStart = 1;
	}
	else if (str[*i] == '"')
	{
		if (!count_words->inSingleQuotes)
			count_words->inDoubleQuotes = !count_words->inDoubleQuotes;
		else
			count_words->wordStart = 1;
	}
	else if (str[*i] == ' ' && !count_words->inSingleQuotes
			&& !count_words->inDoubleQuotes)
	{
		if (count_words->wordStart == 1)
		{
			count_words->word_count++;
			count_words->wordStart = 0;
		}
	}
	else
		count_words->wordStart = 1;
}

int	countWords(char *str)
{
	int i;
	t_count count;

	i = -1;
	initialize_counter(&count);
	while (str[++i])
	{
		count_word_bis(str, &count, &i);
	}
	if (count.wordStart == 1 && !count.inSingleQuotes && !count.inDoubleQuotes)
		count.word_count++;
	return (count.word_count);
}

char	*fix_split(char *str)
{
	int i;
	int len;
	char *fixed;

	i = 0;
	len = ft_strlen(str);
	fixed = malloc(sizeof(char) * len);
	while (i < len - 1)
	{
		fixed[i] = str[i];
		i++;
	}
	fixed[len - 1] = '\0';
	return (fixed);
}

void	fill_new_struct(char *str, t_define *new_struct)
{
	int i;
	char **split_words;
	char *copy;

	i = 0;
	copy = ft_strdup(str);
	while (copy[i])
	{
		if (copy[i] == '\"' || copy[i] == '\'')
			i = skip_quote(copy, i);
		else if (copy[i] == ' ' || copy[i] == '\t' || copy[i] == '\n')
			copy[i] = '\n';
		i++;
	}
	i = 0;
	split_words = ft_split(copy, '\n');
	while (split_words[i])
	{
		new_struct[i].content = ft_strdup(split_words[i]);
		new_struct[i].state = WORD;
		new_struct[i].type = new_struct[i].state;
		i++;
	}
	free_double_array(split_words);
}

void	free_double_array(char **str)
{
	int i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

t_define	*insert_new_struct(t_define *define, t_define *inserted,
		t_list *cmds, int index)
{
	int i;
	int j;
	t_define *final_define;
	final_define = (t_define *)malloc(sizeof(t_define) * cmds->size_cmd);
	i = 0;
	while (i < index)
	{
		final_define[i] = define[i];
		i++;
	}
	j = 0;
	while (j < inserted->size_struct_inserted)
	{
		final_define[i + j] = inserted[j];
		j++;
	}
	while (i < (cmds->size_cmd) - (inserted->size_struct_inserted) + 1)
	{
		final_define[i + j] = define[i];
		i++;
	}
	return (final_define);
}

void	final_struct(t_list *cmds, char **env)
{
	t_list *tmp;
	int i;
	t_define *new_struct;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		i = 0;
		while (i < tmp->size_cmd)
		{
			if (tmp->define[i].dollar == 1)
			{
				tmp->define[i].content = expand_ENV(tmp->define[i].content,
													env);
				if (countWords(tmp->define[i].content) > 1
					&& tmp->define[i].type == FYLE)
				{
					printf("ambiguous redirect\n");
					// free all
					break;
				}
				else if (countWords(tmp->define[i].content) > 1
						&& tmp->define[i].type != FYLE)
				{
					new_struct = malloc(sizeof(t_define)
							* countWords(tmp->define[i].content));
					if (!new_struct)
					{
						// free all
						break;
					}
					initialize_define_inserted(new_struct,
												countWords(tmp->define[i].content));
					fill_new_struct(tmp->define[i].content, new_struct);
					tmp->size_cmd += countWords(tmp->define[i].content) - 1;
					tmp->define = insert_new_struct(tmp->define, new_struct,
							tmp, i);
					i += countWords(tmp->define[i].content) - 1;
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
}

void	cmd_define(t_list *cmds)
{
	int i;
	t_list *tmp;

	tmp = cmds;
	while (tmp)
	{
		tmp->define = malloc(sizeof(t_define) * (tmp->size_cmd));
		if (!(tmp->define))
			return ;
		initialize_define(tmp->define, tmp->size_cmd);
		i = 0;
		while (tmp->cmd[i])
		{
			if (ft_strncmp("<<", tmp->cmd[i], 2) == 0)
			{
				tmp->define[i].state = HEREDOC;
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			else if (ft_strncmp(">>", tmp->cmd[i], 2) == 0)
			{
				tmp->define[i].state = APPEND;
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			else if (ft_strncmp("<", tmp->cmd[i], 1) == 0)
			{
				tmp->define[i].state = RED_IN;
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			else if (ft_strncmp(">", tmp->cmd[i], 1) == 0)
			{
				tmp->define[i].state = RED_OUT;
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			else if (tmp->define[i - 1].state == RED_IN || tmp->define[i
					- 1].state == APPEND ||
						tmp->define[i - 1].state == RED_OUT)
			{
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].state = FYLE;
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			else if (tmp->define[i - 1].state == HEREDOC)
			{
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].state = DELIMITER;
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			else
			{
				tmp->define[i].content = ft_strdup(tmp->cmd[i]);
				tmp->define[i].state = WORD;
				tmp->define[i].type = tmp->define[i].state;
				tmp->define[i].index = i;
			}
			if (ft_strchr(tmp->cmd[i], '$') && tmp->define[i].state != DELIMITER
				&& tmp->define[i].state)
				tmp->define[i].dollar = 1;
			i++;
		}
		tmp = tmp->next;
	}
}

int	check_dollar(char *str, int index)
{
	int i;
	int Squote;
	int Dquote;

	Squote = 0;
	Dquote = 0;
	i = 0;
	while (i < index)
	{
		if (str[i] == '\'' && str[i - 1] != '\\' && Dquote == 0 && Squote == 0)
			Squote = 1;
		else if (str[i] == '\"' && str[i - 1] != '\\' && Dquote == 0
				&& Squote == 0)
			Dquote = 1;
		else if (str[i] == '\'' && str[i - 1] != '\\' && Dquote == 0
				&& Squote == 1)
			Squote = 0;
		else if (str[i] == '\"' && str[i - 1] != '\\' && Dquote == 1
				&& Squote == 0)
			Dquote = 0;
		i++;
	}
	if (i == index && Dquote == 1 && Squote == 0)
		return (0); // Inside Double Quotes
	else if (i == index && Dquote == 0 && Squote == 0)
		return (0); // Not within Quotes
	else if (i == index && Dquote == 0 && Squote == 1)
		return (1); // Inside Single Quotes
	return (0);
}