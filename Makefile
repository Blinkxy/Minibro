
NAME = Minishell
CFLAGS = -Wall -Wextra -Werror
LRFLAG = -lreadline
RM = rm -rf
LIBFT_A = libft/libft.a
CC = gcc
SRC = Minishell.c linked_list_init.c Lexer.c split_redir.c cmd_struct.c redir_array.c \
Errors_free.c split_struct.c general_func/utils.c builtins/ft_exit.c builtins/ft_unset.c \
builtins/ft_echo.c builtins/export_utils.c builtins/ft_export.c builtins/ft_env.c builtins/ft_pwd.c builtins/cd_utils.c \
builtins/ft_cd.c excutions/check_built.c
OBJ = $(SRC:.c=.o)

all: $(NAME) 	

$(NAME): $(OBJ)
	@make -C Libft
	@$(CC) $(OBJ) $(LIBFT_A) -o $(NAME) $(LRFLAG)
	@echo ---program linked---

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:	
	@$(RM) $(OBJ) 

fclean: clean
	@$(RM) $(NAME) libft/*.o $(LIBFT_A)
	@echo ---program cleared---
	@echo ---objects cleared---

re: fclean all

.PHONY: all clean fclean re
