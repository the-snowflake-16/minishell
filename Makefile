NAME = minishell

CC = cc
# -lreadline -fsanitize=address
CFLAGS = -Wall -Wextra -Werror -g 
LDFLAGS = -lreadline -L libft -lft

SRC = token.c token_utils.c creat_list.c sort_input.c env.c main.c first_check_input.c execve.c export_print.c export_add.c export_utils.c export_create.c execution.c signals.c execve_utils.c sort_utils.c redirects.c execution_child.c create_command.c create_utils.c create_init.c create_handel.c create_utils2.c heredok.c heredok2.c execution2.c create_list2.c command.c command_utils.c command_init.c command2.c export_add2.c
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
