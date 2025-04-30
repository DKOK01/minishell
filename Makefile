CFLAGS	= -Wall -Wextra -Werror

SRC		= $(shell ls ./src/input/*.c) \
		  $(shell ls ./src/*.c) 


OBJ		= $(SRC:.c=.o)

NAME	= minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
