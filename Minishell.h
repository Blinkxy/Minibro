/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzoheir <mzoheir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 20:20:00 by mzoheir           #+#    #+#             */
/*   Updated: 2023/09/17 03:05:44 by mzoheir          ###   ########.fr       */
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

extern int gb_ex_st;

typedef enum e_type
{
	WORD = 1,
	HEREDOC,
	APPEND,
	RED_OUT,
	RED_IN,
	FYLE,
	NOT,
	END,
	ENV,
	DELIMITER,
}					t_type;

typedef struct s_define
{
	int				type;
	t_type			state;
	int				index;
	char			*content;
	int				dollar;
	int				size_struct;
	int				size_struct_inserted;
}					t_define;

typedef struct s_redir
{
	int				type;
	char			*red;
	int				*fd;
	char			*file;
	char			*delimiter;
}					t_redir;

typedef struct s_index
{
	int				i;
	int				j;
	int				insinglequotes;
	int				indoublequotes;
	int				inquotes;
	int				len;
	int				index;
}					t_index;

typedef struct s_count
{
	int				word_count;
	int				insinglequotes;
	int				indoublequotes;
	int				wordstart;
}					t_count;

typedef struct s_list
{
    char** cmd;     //  double array of EACH command
	pid_t id;
	int fd_in; // fd_out for ouput redirection 
	int fd_out; // fd_in for input redirection
	int fd[2];  // fd_for herdoc;
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

int					checker_line(char *line);
int					checker_redir(char *line);
int					cnt_cmds(char *line);
int					valid_cmd(char **splitted);
int					checkquotes(char *line);
char				*addnext_pipe(char *str);
void				addnext_pipe_util(t_index *index, char *str, char *new_str);
void				remove_xtra_newlines(char *str);
void				tokenizer(char **str);
t_list				*create_node(char *cmd, int i);
void				add_node_front(t_list *head, char *str, int i);
void				add_prev_list(t_list *cmds);
int					checkquote_index(char *str, int index);
int					check_dollar(char *str, int index);
char				**remove_pipe_pointers(char **str);
char				**split_cmd(char *str);
int					iswhitespace(char c);
void				split_whitespace(char *newstr, t_index *index, char *str);
void				split_red_in(char *newstr, t_index *index);
void				split_append(char *newstr, t_index *index);
void				split_red_out(char *newstr, t_index *index);
void				split_heredoc(char *newstr, t_index *index);
void				split_quotes(char *newstr, t_index *index, char *str);
int					count_cmds(char **str);
int					sizeof_cmd(t_list *cmds);
void				cmd_define(t_list *cmds);
void				add_prev_list(t_list *cmds);

//  ENV Expand
char				*extract_env(char *str);
char				*expand_env(char *str, char **env);
char				*expand_quotes(char *str);
char				*concatenate_char(char *str, char c);
void				initialize_define(t_define *new_struct, int size);
void				initialize_define_inserted(t_define *define, int inserted);
void				initialize_counter(t_count *counter);
void				initialize_index(t_index *index);
int					countwords(char *str);
void				count_word_bis(char *str, t_count *count_words, int *i);
int					skip_quote(char *str, int i);
void				free_double_array(char **str);

// Expand

void				fill_new_struct(char *str, t_define *new_struct);
t_define			*insert_new_struct(t_define *define, t_define *inserted,
						t_list *cmds, int index);
void				free_struct(t_define *define);
void				final_struct(t_list *cmds, char **env);
void				redir_array(t_list *commands);
void				redir_arrayx(t_list *tmp, int *i, int *j);
void				redir_red_in(t_list *tmp, int *i, int *j);
void				redir_red_out(t_list *tmp, int *i, int *j);
void				redir_append(t_list *tmp, int *i, int *j);
void				redir_heredoc(t_list *tmp, int *i, int *j);
void				free_define_and_cmd(t_list *cmds);
void				final_cmd(t_list *cmds);
void				final_remove_quotes(t_list *cmds);

void				free_words(char **words, int count);

//general functions
int ft_size(char **str);
int ft_strcmp(char *s1, char *s2);
void free_tab(char **tab);

// redirections 
void dup_fds(t_list *cmds);
void close_fds(t_list *cmds);

// builtin export
int 	var_export_check(char *env_var);
void get_export_env(t_general *sa);
void double_swap(int i, int j, t_general *sa);
void solo_export(t_general *sa);
int ft_export(t_general *sa, char **cmd);
char	**export_split_var(char *arg);
int check_number_of_wr(char *str, char c);
char *only_name(char *name, char *arg);
char *with_value(char **new_var);
int double_qchek(char *str);

// builtin echo
int	check_n_line(char *line);
int	ft_echo(char **cmd);

// builtin exit
int ft_exit(char **cmd);

// builtin unset
int ft_unset(t_general *sa, char **cmd);

// builtin env
int ft_env(t_general *sa);

// builtin pwd
int ft_pwd(void);

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
void ex_cmd(t_general *sa, t_list *cmd);
int make_red(t_list *cmd, t_general *sa);
int ex_builtins(t_list *cmd, t_general *sa);
void ex_test(t_list *cmd, t_general *sa);
void ex_pipe(t_list *cmd, t_general *sa);
// heredocument
int check_herdoc(t_list *cmds, t_general *sa);
void heredoc(t_list *cmds, t_general *sa, int fd[2]);
int handle_herdoc(t_list *cmds, t_general *sa);
int	hrdc_expand(char *delimiter);

// mak
void pipex(t_list *cmds, t_general *sa);
int numberOf_cmd(t_list *cmds);