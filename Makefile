NAME        = minishell
LIBFT       = ./libft/libft.a
INC         = -I. -I./libft
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
RM          = rm -f
LDFLAGS     = -lreadline


SRC         = minishell.c gc.c lexer.c lexer_utils.c lexer_utils_extra.c env_utils.c env_utils_extra.c \
              expander.c expander_utils.c expander_utils_extra.c syntax_checker.c parser.c executer.c \
              executer_utils.c signals.c builtsin/cd.c builtsin/env.c exec_path_utils.c heredoc_utils.c parser_utils.c\
              builtsin/pwd.c builtsin/echo.c builtsin/export.c builtsin/unset.c exec_finish_utils.c \
			  builtsin/exit.c executer_redir.c heredoc.c

OBJ         = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@make -C ./libft

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(OBJ)
	@make -C ./libft clean

fclean: clean
	$(RM) $(NAME)
	@make -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re