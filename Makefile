CFLAGS	= -Wall -Wextra -Werror

CFLAGS	= -Wall -Wextra -Werror

SRC		= $(shell ls ./src/parser/*.c) \
		  $(shell ls ./src/*.c) \
		  $(shell ls ./src/execution/heredoc/*.c) \
		  $(shell ls ./src/execution/pipe/*.c) \
		  $(shell ls ./src/execution/redirections/*.c) \
		  $(shell ls ./src/execution/*.c) \
		  $(shell ls ./src/builtins/*.c) \
		  $(shell ls ./src/env/*.c) \
		  $(shell ls ./src/expand/*.c) \
		  $(shell ls ./src/lexer/*.c)

OBJ		= $(SRC:.c=.o)

NAME	= minishell

LIBFT_DIR = ./include/libft
LIBFT = ./include/libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJ)  $(LIBFT) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all
