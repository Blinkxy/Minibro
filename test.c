int	skip_quote(char *line, int i)
{
	if (line[i] == '\"')
	{
		i++;
		while (line[i] != '\"')
			i++;
	}
	else if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\'')
			i++;
	}
	return (i);
}

int	split_spaces(t_lexer *lex)
{
	int	i;
    int j;

	i = 0;
    j = 0;
	while (lex->line[i])
	{
		if (lex->line[i] == '\"' || lex->line[i] == '\'')
			i = skip_quote(lex->line, i);
		else if (lex->line[i] == ' ' || lex->line[i] == '\t'
			|| lex->line[i] == '\n')
			lex->line[i] = '\n';
		i++;
	}
	lex->word = ft_split(lex->line, '\n', &lex->curr_wnb);
	free(lex->line);
	return (0);
}