/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 08:01:37 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:24:03 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

extern volatile sig_atomic_t	g_heredoc_interrupted;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_SINGLE_QUOTES,
	TOKEN_DOUBLE_QUOTES
}	t_token_type;

typedef struct s_token
{
	char	**token_arr;
}	t_token;

typedef struct s_parser
{
	char			*word;
	t_token_type	type;
	struct s_parser	*next;
	bool			single_quotes;
}	t_parser;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	int				index;
	struct s_env	*next;
}	t_env;

typedef struct s_redirect
{
	char				*file;
	bool				is_input;
	bool				is_output;
	bool				is_append;
	bool				is_heredoc;
	char				*delimiter;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				*command;
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
	bool				single_quotes;
	int					exit_code;
}	t_command;

typedef struct s_state
{
	t_env	*env;
	int		last_exit_code;
	int		has_next;
	int		heredoc_fd;
	int		fd[2];
	int		in_fd;
	int		pid_count;
}	t_state;

/* token_utils.c */
int		skip_non_content(const char *s, int i, char *quote);
int		count_words(char *s);
char	**ft_splitt(char *s);
char	*extract_word(char *s, int start, int end);
void	free_words(char **rs, int index);

/* token.c */
t_token		*tokenize(char *s);
void		free_token(t_token *token);
int			skip_spaces_and_quotes(char *s, int *i,
			int *in_single, int *in_double);
int			find_word_end(char *s, int i,
			int *in_single, int *in_double);
int			split_loop(char *s, char **rs, int *i, int word_index);

/* check_list.c */
void	check_symbol(t_parser *parser);

/* sort_utils.c */
char	*add_space_before(char *s, int pos);
char	*add_space_after(char *s, int pos);
int		is_special(char c);
int		is_double_operator(char *str, int i, int len);

/* sort_input.c */
char	*filter(char *s);

/* create_list.c */
char		*strjoin_and_free(char *s1, const char *s2);
char		*strjoin_and_free_char(char *s1, char c);
void		free_list(t_parser *head);
t_command	*create_command(t_parser *parser);
t_parser	*create_list(char **ss, t_state *state);
char		*get_parametr(char *s);
int			first_quoter(char *s);

/* create_handel.c */
bool	handle_redirections(t_parser **parser, t_command *cmd);
bool	cheak_token(t_parser *parser);
int		count_args_until_pipe(t_parser *parser);
bool	handle_heredoc_append(t_parser **parser, t_command *cmd);
bool	handle_in_out_redirs(t_parser **parser, t_command *cmd);

/* create_utils2.c */
void	free_comand(t_command *command);
void	free_redirect(t_redirect *redirect);

/* create_init.c */
void	init_comand(t_command *comand);
void	init_redirin(t_command *cmd, t_token_type inp, char *file);
void	init_heredok(char *delimiter, t_command *cmd);
void	init_append(t_token_type inp, char *file, t_command *cmd);
void	init_redir_out(t_token_type inp, char *file, t_command *cmd);

/* create_utils.c */
void	free_list(t_parser *head);
int		chech_symbol(char *s);
char	*delete_quotes(const char *s, char q);

/* env.c */
t_env	*init_env(char **s);
void	free_env(t_env *env);
t_env	*add_left_right(char *s);

/* first_check_input.c */
int		incorect_input(char *s);

/* execve_utils.c */
int		count_env(t_env *env);
char	**t_env_to_envp(t_env *env);
void	free_envp(char **envp);
void	ft_free_array(char **arr);
char	*get_path_from_env(t_env *env);

/* execve.c */
void	start_execve(char **args, t_state *state);

/* export_print.c */
void	export_print(t_env *my_env);

/* export_add.c */
void	export_add(t_env *my_env, t_command *command);
int		check_key_in_env(t_env *my_env, char *s);
t_env	*find_key(t_env *my_env, char *s);
int		incorect_input_for_key(char *s);
char	*split_by_equal_key(char *s);
int		count_value_length(const char *s);
char	*get_value_after_equal(const char *s);

/* export_utils.c */
int		check_str_for_export_add_after_equal(char *s);
int		count_exported(t_env *my_env);
int		compare_env(const void *a, const void *b);

/* export_create.c */
int		export_create(t_env *my_env, t_command *command);
int		unset(t_env **my_env, char *s);

/* executions.c */
void	execute_pipeline(t_command *cmd, t_state *state);
int		handle_redirectionss(t_command *cmd);
int		execute_builtin_last(t_command *cmd, t_state *state);
void	setup_child_input(t_state *state);
void	child_process(t_command *cmd, t_state *state, int has_next);
void	setup_child_output(t_state *state, int has_next);
void	parent_process_cleanup(t_state *state, int has_next);
int		fork_and_execute(t_command *cmd, t_state *state,
			int has_next, pid_t *pids);

/* main.c */
void	cmp_input(t_command *command, t_state *state);
int		handle_builtin(t_command *command, t_state *state);

/* signals.c */
void	sigint_handler(int signo);
void	update_exit_code(t_state *s, int status);

/* redirects.c */
int		redirect_output(const char *file);
int		redirect_input(const char *file);
int		redirect_append(const char *file);
int		is_parent_builtin(const char *cmd);
int		setup_pipe(t_state *state, int has_next);

/* heredok.c */
void	heredoc_sigint_handler(int sig);
int		process_heredoc_line(char *line, const char *delimiter, int pipe_fd);
void	heredoc_child_process(const char *delimiter, int pipe_fd[2]);
int		heredoc_parent_process(pid_t pid, int *pipe_fd);
int		heredok(const char *delimiter);

/* heredok2.c */
int		write_line_to_pipe(int pipe_fd, char *line);
int		handle_child_status(int status, int pipe_fd);
int		handle_heredoc(t_command *cmd, t_state *state);

/* create_list2.c */
char		*strjoin_and_free(char *s1, const char *s2);
char		*strjoin_and_free_char(char *s1, char c);
t_parser	*creat_node(char *s);
t_parser	*append_node(t_parser **head, t_parser **current,
			t_parser *new);
int			first_quoter(char *s);

/* command.c */
int		handle_exit(t_command *command, t_state *state);
int		ft_pwd(void);
int		change_cwd(char **swd);
int		echo(t_command *command);
int		is_builtin(const char *cmd);

/* command2.c */
void	print_env(t_env *my_env);
int		cd_to_home(void);
int		cd_to_path(char *path);
int		change_cwd(char **swd);

/* command_utils.c */
void	exit_with_numeric_error(t_command *cmd, t_state *state);
int		exit_with_too_many_args(t_state *state);
void	clean_and_exit(t_command *cmd, t_state *state, int code);
int		count(char **ss);
int		is_numeric(const char *str);

/* command_init.c */
int		handle_builtin(t_command *command, t_state *state);
void	cmp_input(t_command *command, t_state *state);
int		validate_redirections(t_parser *parser);

#endif
