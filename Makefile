
NAME = Minishell
CFLAGS = -Wall -Wextra -Werror -g 
LRFLAG = -lreadline
RM = rm -rf
LIBFT_A = libft/libft.a
CC = gcc
SRC = Minishell.c linked_list_init.c Lexer.c split_redir.c cmd_struct.c redir_array.c redir_array_util.c\
Errors_free.c split_struct.c general_func/utils.c builtins/ft_exit.c builtins/ft_unset.c env_handling.c\
builtins/ft_echo.c builtins/export_utils.c builtins/ft_export.c builtins/ft_env.c builtins/ft_pwd.c builtins/cd_utils.c \
builtins/ft_cd.c excutions/check_built.c excutions/pipe.c excutions/ex.c excutions/redirections.c initializers.c excutions/handler.c\
split_redir_util.c split_redir_util_bis.c cmd_define.c cmd_define_utils.c  excutions/signals.c
OBJ = $(SRC:.c=.o)

all: $(NAME) 	

$(NAME): $(OBJ)
	@make -C libft
	@$(CC)  $(OBJ) -o $(NAME) $(LIBFT_A) $(LRFLAG) -L/Users/mdouzi/goinfre/homebrew/opt/readline/lib -I/Users/mdouzi/goinfre/homebrew/opt/readline/include -lreadline

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:	
	$(RM) $(OBJ) 

fclean: clean
	$(RM) $(NAME) libft/*.o $(LIBFT_A)

re: fclean all

.PHONY: all clean fclean re
