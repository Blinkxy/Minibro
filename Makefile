
NAME = Minishell
CFLAGS = -Wall -Wextra -Werror
LRFLAG = -lreadline
RM = rm -rf
LIBFT_A = libft/libft.a
CC = gcc
SRC = Minishell.c Tokenizer.c linked_list_init.c Lexer.c Lexer_bis.c
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
