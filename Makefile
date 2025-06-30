CC		= cc
CFLAGS	= -Wall -Wextra -Werror

GREEN = \033[1;32m
BLUE = \033[1;34m
PURPLE = \033[1;35m
YELLOW = \033[1;33m
CYAN = \033[1;36m
RED = \033[1;31m
RESET = \033[0m

SRC		= src/main.c \
		  src/parser/parser.c \
		  src/parser/parser_utils.c \
		  src/parser/parser_validation.c \
		  src/lexer/lexer.c \
		  src/lexer/lexer_segments.c \
		  src/lexer/lexer_segments_utils.c \
		  src/lexer/lexer_token_status.c \
		  src/expand/expander.c \
		  src/expand/expander_utils.c \
		  src/execution/execution.c \
		  src/execution/execution_utils.c \
		  src/execution/run_builtins.c \
		  src/execution/signals.c \
		  src/execution/signals_handlers.c \
		  src/execution/utils.c \
		  src/execution/heredoc/heredoc.c \
		  src/execution/heredoc/heredoc_expand.c \
		  src/execution/heredoc/heredoc_fd.c \
		  src/execution/heredoc/heredoc_io.c \
		  src/execution/heredoc/heredoc_process.c \
		  src/execution/pipe/pipe.c \
		  src/execution/pipe/pipe_utils.c \
		  src/execution/redirections/execution_builtins.c \
		  src/execution/redirections/execution_redirections.c \
		  src/execution/redirections/infile.c \
		  src/execution/redirections/outfile.c \
		  src/execution/redirections/redirections.c \
		  src/builtins/cd.c \
		  src/builtins/echo.c \
		  src/builtins/env.c \
		  src/builtins/exit.c \
		  src/builtins/export.c \
		  src/builtins/export_utils.c \
		  src/builtins/pwd.c \
		  src/builtins/unset.c \
		  src/env/env_to_envp.c \
		  src/env/environment.c

OBJ		= $(SRC:.c=.o)

NAME	= minishell

LIBFT_DIR = ./include/libft
LIBFT = ./include/libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) -s
	@$(CC) $(CFLAGS) $(OBJ)  $(LIBFT) -o $(NAME) -lreadline
	@echo "$(GREEN)Minishel compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean -s
	@rm -f $(OBJ)
	@echo "$(YELLOW)Cleaning object files$(RESET)"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean -s
	@rm -f $(NAME)
	@echo "$(RED)Minishell cleaned successfully!$(RESET)"

re: fclean all
	@echo "$(BLUE)Minishell rebuilt successfully!$(RESET)"
