NAME = minishell

CC = cc
# -lreadline
CFLAGS =  -Wextra -Werror -g
LDFLAGS = -lreadline -L libft -lft

SRC = token.c token_utils.c creat_list.c sort_input.c env.c main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/  
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	@make clean -C libft/ 

fclean: clean
	rm -f $(NAME)
	@make fclean -C libft/ 

re: fclean all

.PHONY: all clean fclean re
