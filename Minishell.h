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
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>

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

	int has_herdoc; // falg indicating if the cmd has a herdoc
	int herdoc_content_fd; // fd for herdoc content
    char** cmd;     //  double array of EACH command
	int fd_in;
	int fd_out;
    int index;		// index of the command
	int size_cmd;	// how many pointers in each command
	t_define *define; // each pointer of cmd is in a struct for define/expand
	int red_nb;		// number of total redirection in this command
	t_redir *redir;	// array of all the redir in this command to send to exec
	char** final_cmd;	// command + arg(if existent) to send to exec
	struct s_list *prev;
    struct s_list *next;
}           t_list;

typedef struct s_general
{
	int ex_status;
	char **env;
	char **env_export;
	t_list *cmds;

}	t_general;

//   PARSING UTILS

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
void 	add_prev_list(t_list *cmds);
int 	checkQuoteIndex(char* str, int index);
int 	check_dollar(char *str, int index);
char 	**removePipePointers(char** str);
char** 	split_cmd(char* str);
int 	isWhitespace(char c);
int 	count_cmds(char **str);
int 	sizeof_cmd(t_list *cmds);
void 	cmd_define(t_list *cmds);
void add_prev_list(t_list *cmds);

		//  ENV Handling		
char 	*extract_ENV(char *str);
char 	*expand_ENV(char *str, char **env);
char	*Expand_quotes(char* str);
char* 	concatenate_char(char* str, char c);

void 	initialize_define(t_define *new_struct, int size);
int 	countWords(char* str);
int		skip_quote(char *str, int i);
char	**splitWords(char* str, int* wordCount);
void	free_double_array(char **str);

void 	fill_new_struct(char *str, t_define *new_struct);
t_define *insert_new_struct(t_define *define, t_define *inserted, t_list *cmds, int index);
void 	free_struct(t_define *define);
void 	final_struct(t_list *cmds, char **env);
void    redir_array(t_list *commands);
void 	free_define_and_cmd(t_list *cmds);
void    final_cmd(t_list *cmds);
void    final_remove_quotes(t_list *cmds);

void free_words(char** words, int count);

//general functions
int ft_size(char **str);
int ft_strcmp(char *s1, char *s2);
void free_tab(char **tab);

// redirections 


// builtin export
int 	var_export_check(char *env_var);
void get_export_env(t_general *sa);
void double_swap(int i, int j, t_general *sa);
void solo_export(t_general *sa, int fd);
int ft_export(t_general *sa, char **cmd, int fd);
char	**export_split_var(char *arg);
int check_number_of_wr(char *str, char c);
char *only_name(char *name, char *arg);
char *with_value(char **new_var);
int double_qchek(char *str);

// builtin echo
int	check_n_line(char *line);
int	ft_echo(char **cmd, int fd);

// builtin exit
int ft_exit(char **cmd, t_general * sa);

// builtin unset
int ft_unset(t_general *sa, char **cmd);

// builtin env
int ft_env(t_general *sa, int fd);

// builtin pwd
int ft_pwd(int fd);

//builtin cd
int ft_cd(t_general *sa, char **cmd);
int cd_home(t_general *sa);
int update_oldpwd(t_general *sa);
char *get_path_env(char *env);
char *env_join(char *s1, char *s2);
int remplace_env_var(char **str, int index, char *name);
int compare_env_var(char *str, char *var);
int get_env_var(char **env, char *var);


//exuction functions
int handle_builtins(t_list *cmds, t_general *sa);
int ex_minishell(t_list *cmd, t_general *sa);
int is_builtin(char **args);
void ex_cmd(t_general *sa, char **cmd);
// pipe
void pipex(t_list *cmds, t_general *sa);
int numberOf_cmd(t_list *cmds);