/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:20:00 by mzoheir           #+#    #+#             */
/*   Updated: 2023/08/16 13:59:32 by marvin           ###   ########.fr       */
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
	WORD = 1,   // argument of command or ENV
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

typedef struct s_define
{
	int type;
	t_type state;
	int index;
	char *content;
	int dollar;
	int size_struct;
	int size_struct_inserted;
}	t_define;

typedef struct s_redir
{
	int type;
    char *red; // < or > or << or >>
	int *fd;
	char *file;
	char *delimiter;	
}	t_redir;

typedef struct s_list
{
    char** cmd;     //  double array of EACH command
	char *raw_cmd;	// raw command with no split
    int index;		// index of the command
	int size_cmd;	// how many pointers in each command
	t_define *define; // each pointer of cmd is in a struct for define/expand
	int red_nb;		// number of total redirection in this command
	t_redir *redir;	// array of all the redir in this command to send to exec
	char** final_cmd;	// command + arg(if existent) to send to exec
    struct s_list *next;
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
int 	checkQuoteIndex(char* str, char* charPtr);
char 	**removePipePointers(char** str);
char** 	split_cmd(char* str);
int 	isWhitespace(char c);
int 	count_cmds(char **str);
int 	sizeof_cmd(t_list *cmds);
void 	cmd_define(t_list *cmds);
int 	remplace_env_var(char **str, int index, char *name);
int 	compare_env_var(char *str, char *var);
int 	get_env_var(char **env, char *var);
char 	*expand_ENV(char *str, char **env);
char	*Expand_quotes(char* str);
char 	**expand_all(t_list *cmds);
void 	initialize_define(t_list *cmds);
int 	countWords(char* str);
char	**splitWords(char* str, int* wordCount);
void 	fill_new_struct(char *str, t_define *new_struct, t_list *cmds);
void 	insert_new_struct(t_define *define, t_define *inserted, t_list *cmds, int index);
void 	free_struct(t_define *define);
void 	*final_struct(t_list *cmds, char **env);
t_redir *redir_array(t_list *commands);