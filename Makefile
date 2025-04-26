NAME = minishell

CC = cc
# -lreadline -fsanitize=address
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -lreadline -L libft -lft

SRC = token.c token_utils.c creat_list.c sort_input.c env.c main.c first_check_input.c execve.c export_print.c export_add.c export_utils.c export_create.c
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
