/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:20:00 by mzoheir           #+#    #+#             */
/*   Updated: 2023/07/19 22:24:53 by marvin           ###   ########.fr       */
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
	WORD,   // argument of command
	COMMAND, // first word of command
	HEREDOC,  // <<
	APPEND,   // >>
	RED_OUT,      // >
	RED_IN,		// <
	FYLE,       // argument of redir
	NOT,      // string
	END,      // end of cmd
	ENV,
	DELIMITER, // of heredoc
}   t_type;

typedef enum e_state
{
	IN_DQUOTE,
	IN_SQUOTE,
	GENERAL,
}	t_state;

typedef struct s_redir
{
    char* content; // < or > or << or >>
	t_type type;	// check ENUM
	int *fd;		
	int dollar;		//
	char *file;		
}	t_redir;

typedef struct s_list
{
    char** cmd;      //  double array of each command
	char *raw_cmd;	// raw command with no split
    int index;		// index of the command
	int len;		// lenght of command
	t_state state;	// state (Squote or Dquote or none)
	int red_nb;		// number of total redirection in this command
    struct s_list *next;
	t_redir *redir;	// array of all the redir in this command to send to exec
	char** final_cmd;	// command + arg(if existent) to send to exec
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