/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 13:17:26 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 14:25:36 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words_in_arr(char **ss)
{
	int	i;

	i = 0;
	while (ss[i])
		i++;
	return (i);
}

int	is_numeric(const char *str)
{
	if (!str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	handle_exit(t_command *command, t_state *state)
{
	int				argc;
	long long		num;
	unsigned char	exit_code;

	argc = count_words_in_arr(command->args);
	if (argc == 1)
	{
		free_comand(command);
		free_env(state->env);
		exit(state->last_exit_code);
	}
	if (!is_numeric(command->args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			command->args[1]);
		free_comand(command);
		free_env(state->env);
		exit(2);
	}
	if (argc > 2)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		state->last_exit_code = 1;
		return (1);
	}
	num = strtoll(command->args[1], NULL, 10);
	exit_code = (unsigned char)(num % 256);
	free_comand(command);
	free_env(state->env);
	exit(exit_code);
}

int	ft_pwd(void)
{
	char	path[4096];

	if (getcwd(path, sizeof(path)) != NULL)
		printf("%s\n", path);
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}

int	change_cwd(char **swd)
{
	int		count;
	char	*home;

	count = 0;
	while (swd[count])
		count++;
	if (count == 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	else if (count > 2)
	{
		fprintf(stderr, "cd: too many arguments\n");
		return (1);
	}
	else
	{
		if (chdir(swd[1]) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	return (0);
}

int echo(t_command *command)
{
    int i = 1;
    int newline = 1;

    if (!command || !command->args)
        return 1; // Ошибка: нет аргументов

    if (command->args[1] && !ft_strcmp(command->args[1], "-n"))
    {
        newline = 0;
        i = 2;
    }

    while (command->args[i])
    {
        // single_quotes тут не влияет, одинаково printf("%s")
        printf("%s", command->args[i]);
        if (command->args[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
        printf("\n");

    // Освобождение команды лучше делать вне echo, чтобы не было двойного free
    // free_comand(command);

    return 0;
}


int	is_builtin(const char *cmd)
{
	return (!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "echo") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") ||
		!ft_strcmp(cmd, "env"));
}

int	handle_builtin(t_command *command, t_state *state)
{
	int	exit_code;

	if (!ft_strcmp(command->command, "exit"))
		exit_code = handle_exit(command, state);
	else if (!ft_strcmp(command->command, "pwd"))
		exit_code = ft_pwd();
	else if (!ft_strcmp(command->command, "cd"))
		exit_code = change_cwd(command->args);
	else if (!ft_strcmp(command->command, "export")
		&& count_words_in_arr(command->args) > 1)
		exit_code = export_create(state->env, command);
	else if (!ft_strcmp(command->command, "export"))
		export_print(state->env);
	else if (!ft_strcmp(command->command, "env"))
		print_env(state->env);
	else if (!ft_strcmp(command->command, "unset"))
	{
		if (!command->args[1])
			exit_code = 0;
		else
			exit_code = unset(&state->env, command->args[1]);
	}
	else if (!ft_strcmp(command->command, "echo"))
		exit_code = echo(command);
	return (exit_code);
}

void	cmp_input(t_command *command, t_state *state)
{
	if (command->command && is_builtin(command->command))
		state->last_exit_code = handle_builtin(command, state);
	else if (command->command)
		start_execve(command->args, state);
}

void	start_token(char *input, t_state *state)
{
	char		*sort_input;
	t_token		*token;
	t_parser	*parser_list;
	t_command	*command;

	if (!incorect_input(input))
	{
		sort_input = filter(input);
		token = tokenize(sort_input);
		free(sort_input);
		parser_list = create_list(token->token_arr, state);
		free_token(token);
		command = create_command(parser_list);
		if (command != NULL)
		{
			execute_pipeline(command, state);
			free_comand(command);
		}
		else
			free_list(parser_list);

		token = NULL;
	}
}

void	init_minishell(char **env)
{
	t_state	state;
	char	*input;

	state.env = init_env(env);
	state.last_exit_code = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell❄️   ");
		if (!input)
			break ;
		if (input)
			add_history(input);
		start_token(input, &state);
		free(input);
	}
	free_env(state.env);
}


int	main(int argc, char **argv, char **env)
{
	(void)argv;
	(void)argc;
	init_minishell(env);
	return (0);
}
