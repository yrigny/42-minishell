CC		= cc

CFLAGS		= -Wall -Wextra -Werror

FT_DIR		= ./libft

LFLAGS 		= -L$(FT_DIR) -lft -lreadline

CFILES		= main.c minishell.c minishell_utils.c \
			tokenizer.c tokenizer_utils.c \
			syntax_checker.c \
			token_parser.c token_parser_utils.c \
			pre_expand.c pre_expand_utils.c \
			exec_manager.c redir_manager.c \
			pipex.c execution.c \
			helper_functions.c builtins.c

OFILES		= $(CFILES:.c=.o)

NAME		= minishell

GNL			= 0

GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
MAGENTA=\033[0;35m
NC=\033[0m

all: $(NAME)

$(NAME): $(OFILES)
	@cd ./libft && make > /dev/null
	@echo "$(GREEN)Compiled libft ✅ $(NC)"
	@$(CC) $(OFILES) -o $(NAME) $(LFLAGS)
	@echo "$(GREEN)Compiled $(NAME) ✅ $(NC)"

$(OFILES): %.o: %.c minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@ -D GNL=$(GNL)

clean:
	@cd ./libft && make clean > /dev/null
	@rm -f $(OFILES)
	@echo "$(MAGENTA)Cleaned object files ✅ $(NC)"

fclean: clean
	@cd ./libft && make fclean > /dev/null
	@echo "$(MAGENTA)Cleaned libft ✅ $(NC)"
	@rm -f $(NAME)
	@echo "$(MAGENTA)Cleaned $(NAME) ✅ $(NC)"

re: fclean all

.PHONY : all clean fclean re
