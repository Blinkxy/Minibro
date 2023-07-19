/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:20:00 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/19 21:56:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>


typedef enum e_type
{
	WORD = 1,
	COMMAND,
	HEREDOC,  // <<
	APPEND,   // >>
	RED_OUT,      // >
	RED_IN,
	FYLE,       // <
	NOT,      // string
	END,      // end of cmd
	ENV,
	DELIMITER,
}   t_type;

typedef enum e_state
{
	IN_DQUOTE = 1,
	IN_SQUOTE,
	GENERAL,
}	t_state;

typedef struct s_redir
{
    char* content;
	t_type type;
	int *fd;
	int dollar;
	char *file;
}	t_redir;

typedef struct s_list
{
    char** cmd;
	char *raw_cmd;
    int index;
	int len;
	t_state state;
	int red_nb;
    struct s_list *next;
	t_redir *redir;
	char** final_cmd;
}           t_list;

int     checker_line(char *line);
int 	start_pipe(char *s);
int     cnt_cmds(char *line);
int     valid_cmd(char **splitted);
int     checkQuotes(char* line);
char    *addnext_pipe(char* str);
void	removeExtraNewlines(char* str);
void    tokenizer(char **str);
t_list *createNode(char* cmd, int i);
void 	addNodeFront(t_list *head, char* str, int i);
int    	ENV_checker(char *str);
int 	checkQuoteIndex(char* str, char* charPtr);
char 	**removePipePointers(char** str);
char** 	split_cmd(char* str);
int 	isWhitespace(char c);
int 	count_cmds(char **str);