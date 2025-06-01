#include "minishell.h"

void print_list(t_parser *parser)
{
    int i = 0;
    while (parser != NULL)
    {
        printf("token %d ",i);
        printf ("%s ", parser->word);
        if(parser->type == TOKEN_WORD)
            printf("type word\n");
       if(parser->type == TOKEN_PIPE)
            printf("type pipe\n");
        if(parser->type == TOKEN_REDIR_IN)
            printf("type redir in <\n");
        if(parser->type == TOKEN_REDIR_OUT)
            printf("type redir out >\n");
        if(parser->type == TOKEN_REDIR_APPEND)
            printf("type redie append >>\n");
        if(parser->type == TOKEN_REDIR_HEREDOC)
            printf("type redir heredoc <<\n");  
        parser = parser->next;

        i++;
    }
}

void free_list(t_parser *head)
{
    t_parser *current = head;
    t_parser *next;
    
    while (current != NULL)
    {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}
int	chech_symbol(char *s)
{
	if (!ft_strcmp(s, "<<"))
		return (TOKEN_REDIR_HEREDOC);
	if (!ft_strcmp(s, ">>"))
		return (TOKEN_REDIR_APPEND);
	if (!ft_strcmp(s, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(s, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(s, "|"))
		return (TOKEN_PIPE);
    // if(ft_strchr(s, '"'))
    // {
    //     printf("find qoutes");
    //     return(TOKEN_DOUBLE_QUOTES); 
    // }
  
	return (TOKEN_WORD);
}

char *delete_quotes(const char *s, char q)
{
    int len = ft_strlen(s);
    int i = 1;
    int j = 0;
    char *res;
    int count = 0;
    while (s[i])
    {
        if(s[i] == q)
            count++;
        i++;
    }

    res = malloc(len - count + 1);
    if (!res)
        return NULL;
    i = 0;
    while (j < len - count)
    {
        if (s[i]== q && s[i+1] != q)
            i++;
        else if (s[i] == q && s[i+1] == q)
            i += 2;
        res[j] = s[i];
        i++;
        j++;
    }
    res[j] = '\0';

    return res;
}



t_parser *creat_node(char *s)
{
    t_parser *new = malloc(sizeof(t_parser));
    if(!new)
        return NULL;
    new->word = ft_strdup(s);
    new->type = chech_symbol(s);
    new->next = NULL;
    return (new);
}

t_parser *append_node(t_parser **head, t_parser **current, t_parser *new_node)
{
    if (!new_node)
        return NULL;
    if (!*head)
    {
        *head = new_node;
        *current = new_node;
    }
    else
    {
        (*current)->next = new_node;
        *current = new_node;
    }
    return new_node;
}

char *get_parametr(char *s)
{
    char *res = malloc(sizeof(char) * (ft_strlen(s)));
    int i = 0;
    int j = 1;
    while (s[j])
    {
        res[i++] = s[j++];
    }
    res[i] = '\0';
    return res;
}
int first_quoter(char *s)
{
    int i = 0;
    while (s[i])
    {
        if(s[i] == '\'')
            return 1;
        else if (s[i] == '"')
        {
            return 2;
        }
        
        i++;
    }
    return 0;
}
char *strjoin_and_free(char *s1, const char *s2)
{
    char *new = ft_strjoin(s1, s2);
    free(s1);
    return new;
}

char *strjoin_and_free_char(char *s1, char c)
{
    char temp[2] = {c, '\0'};
    return strjoin_and_free(s1, temp);
}

t_parser *create_list(char **ss, t_state *state)
{
    int i = 0;
    t_parser *head = NULL;
    t_parser *current = NULL;

    while (ss[i])
    {
        char *word = ss[i];
        t_parser *new_node;
        bool is_single_quote = ft_strchr(word, '\'') && first_quoter(word) == 1;
        bool is_double_quote = ft_strchr(word, '"') && first_quoter(word) == 2;

        if (is_single_quote)
        {
            // Remove single quotes but skip expansion
            char *clean = delete_quotes(word, '\'');
            new_node = creat_node(clean);
            free(clean);
        }
        else
        {
            // Remove double quotes or duplicate as-is
            char *clean = is_double_quote ? delete_quotes(word, '"') : ft_strdup(word);

            // Expand $VAR only if not single-quoted
            char *expanded = ft_calloc(1, 1);
            int j = 0;
            while (clean[j])
            {
                if (clean[j] == '$')
                {
                    j++;
                    if (clean[j] == '\0')
                    {
                        expanded = strjoin_and_free(expanded, "$");
                        break;
                    }

                    if (clean[j] == '?')
                    {
                        // Optional: insert exit status
                        // sstate->last_exit_code = 0;
                        char *exit_str = ft_itoa(state->last_exit_code);
                        expanded = strjoin_and_free(expanded, exit_str);
                        free(exit_str);
                        j++;
                    }
                    else
                        {
                        int start = j;
                        while (ft_isalnum(clean[j]) || clean[j] == '_')
                            j++;
                        if (j > start)
                        {
                            char *key = ft_substr(clean, start, j - start);
                            t_env *found = find_key(state->env, key);
                            if (found)
                                expanded = strjoin_and_free(expanded, found->value);
                            free(key);
                        }
                        else
                        {
                            expanded = strjoin_and_free_char(expanded, '$');
                        }
                    }
                }
                else
                {
                    expanded = strjoin_and_free_char(expanded, clean[j]);
                    j++;
                }
            }

            new_node = creat_node(expanded);
            free(expanded);
            free(clean);
        }

        if (!append_node(&head, &current, new_node))
            return NULL;

        i++;
    }

    return head;
}


int count_argv(char **s, int position)
{
    int i = position;
    int count = 0;
    while (s[i])
    {
        if(ft_strcmp(s[i], "|"))
            return count;
        count++;
        i++;
    }
    return count;
}

int count_args(char **s, int position)
{
    int count = 0;
    while (s[position] && ft_strcmp(s[position], "|") != 1)
    {
        count++;
        position++;
    }
    return count;
}

// void print_command(t_command *command)
// {

//     while (command)
//     {
//         int i = 0;
//         while (command->args && command->args[i])
//         {
//             printf("%s\n", command->args[i]);
//             i++;
//         }
//         command = command->next;
//     }
// }
#include <stdio.h>

void print_redirect(t_redirect *redir)
{
    if (!redir)
    {
        printf("  No redirect\n");
        return;
    }
    printf("  Redirect:\n");
    if (redir->file != NULL) {
        printf("Redirect file: %s\n", redir->file);
    } else {
        fprintf(stderr, "Error: Redirect file is NULL!\n");
    }
    printf("    is_input: %s\n", redir->is_input ? "true" : "false");
    printf("    append: %s\n", redir->is_append ? "true" : "false");
    printf("    is_heredoc: %s\n", redir->is_heredoc ? "true" : "false");
    if(redir->is_heredoc)
        printf("delimiter %s\n", redir->delimiter);
}

void print_command(t_command *cmd)
{
    int i;

    while (cmd)
    {
        printf("Command:\n");
        if (cmd->args)
        {
            i = 0;
            while (cmd->args[i])
            {
                printf("  arg[%d]: %s\n", i, cmd->args[i]);
                i++;
            }
        }
        else
            printf("  No args\n");

        printf("Input redirect:\n");
        print_redirect(cmd->redirects);

        printf("\n---\n");
        cmd = cmd->next;
    }
}


void init_comand(t_command *comand)
{
    comand->args = NULL;
    comand->redirects = NULL;
    comand->next = NULL;
}
// void free_comand(t_command *command)
// {
//     t_command *tmp;

//     tmp = command;
//     t_command *curent;
//     while (tmp)
//     {
//         curent = tmp->next;
//         free(tmp);
//         tmp = curent;
//     }
    
// }

// free_parser:
// free_parser:
void free_comand(t_command *command)
{
    t_command *tmp;
    int i;
 
    while (command)
    {
        tmp = command->next;

        if (command->command)
            free(command->command);

        if (command->args)
        {
            i = 0;
            while (command->args[i])
            {
                free(command->args[i]);
                i++;
            }
            free(command->args);
        }

        if (command->redirects)
        {
            // if(command->redirects->delimiter)
            //     free(command->redirects->delimiter);
            if (command->redirects->file)
                free(command->redirects->file);
            free(command->redirects);
        }
        free(command);
        command = tmp;
    }
    free(command);
}



void init_redirin(t_command *cmd, t_token_type inp, char *file)
{
    if (!cmd->redirects)
    {
        cmd->redirects = malloc(sizeof(t_redirect));
        if (!cmd->redirects)
            return;
        // cmd->input->file = file;
    }

    if(inp == TOKEN_REDIR_IN)
    {
        cmd->redirects->file = ft_strdup(file);
        cmd->redirects->is_heredoc = false;
        cmd->redirects->is_append = false;
        cmd->redirects->is_input = true;
        cmd->redirects->is_output = false;
    }
}

void init_heredok(char *delimiter, t_command *cmd)
{
    if (!cmd->redirects)
    {
        cmd->redirects = malloc(sizeof(t_redirect));
        if (!cmd->redirects)
            return;
    }
    cmd->redirects->file = NULL;

    cmd->redirects->is_heredoc = true;
    cmd->redirects->is_append = false;
    cmd->redirects->is_input = false;
    cmd->redirects->is_output = false;
    cmd->redirects->delimiter = ft_strdup(delimiter);

}
void init_append(t_token_type inp, char *file, t_command *cmd)
{
    if (!cmd->redirects)
    {
        cmd->redirects = malloc(sizeof(t_redirect));
        if (!cmd->redirects)
            return;
    }

    cmd->redirects->file = ft_strdup(file);

    if(inp == TOKEN_REDIR_APPEND)
    {
        cmd->redirects->is_heredoc = false;
        cmd->redirects->is_output = false;
        cmd->redirects->is_input = false;
        cmd->redirects->is_append = true;
    }
}
void init_redir_out(t_token_type inp, char *file, t_command *cmd)
{
        if (!cmd->redirects)
        {
            cmd->redirects = malloc(sizeof(t_redirect));
            if (!cmd->redirects)
                return;
        }
    
        cmd->redirects->file = ft_strdup(file);
    
        if(inp == TOKEN_REDIR_OUT)
        {
            cmd->redirects->is_heredoc = false;
            cmd->redirects->is_append = false;
            cmd->redirects->is_input = false;
            cmd->redirects->is_output = true;
        }
   
}

bool cheak_token(t_parser *parser)
{
    
    while (parser)
    {
        if (!ft_strcmp(parser->word, "|") && parser->next == NULL)
        {
            perror("error");
            return false;
        }
        parser = parser->next;
    }
    return true;
}
t_command *create_parser(t_parser *parser)
{
    t_command *head = NULL;
    t_command *prev = NULL;

    while (parser)
    {
        // Подсчёт количества аргументов до пайпа
        t_parser *tmp = parser;
        int i = 0;
        while (tmp && tmp->type != TOKEN_PIPE)
        {
            i++;
            tmp = tmp->next;
        }

        // Создание новой команды
        t_command *cmd = malloc(sizeof(t_command));
        if (!cmd)
            return NULL;
        init_comand(cmd);

        cmd->args = malloc(sizeof(char *) * (i + 1));
        if (!cmd->args)
            return NULL;

        int j = 0;
        while (parser && parser->type != TOKEN_PIPE)
        {
            if (((parser->type == TOKEN_REDIR_APPEND || parser->type == TOKEN_REDIR_HEREDOC || parser->type == TOKEN_REDIR_IN || parser->type == TOKEN_REDIR_OUT) && parser->next == NULL))
            {
                perror("error\n");
                return NULL;
            }
                
            if (parser->type == TOKEN_REDIR_HEREDOC && parser->next) // <<
            {
                cmd->command = NULL;
                init_heredok(parser->next->word, cmd);
                parser = parser->next->next;
                continue;
            }
            if (parser->type == TOKEN_REDIR_APPEND ) // >>
            {
                init_append(parser->type, parser->next->word, cmd);
                parser = parser->next->next;
                continue;
            }
            else if (parser->type == TOKEN_REDIR_OUT) // >
            {
                init_redir_out(parser->type, parser->next->word, cmd);
                parser = parser->next->next;
                continue;
            }
            else if (parser->type == TOKEN_REDIR_IN) // <
            {
                init_redirin(cmd, parser->type, parser->next->word);
                parser = parser->next->next;
                continue;
            }

            // Аргументы команды (включая саму команду)
            if (parser->word != NULL)
            {
                cmd->args[j] = ft_strdup(parser->word);
                if (parser->single_quotes)
                    cmd->single_qoutes = true;
                else
                    cmd->single_qoutes = false;
                j++;
            }

            parser = parser->next;
        }
        cmd->args[j] = NULL;

        // Устанавливаем команду, если аргументы не пусты
        if (cmd->args[0])
            cmd->command = ft_strdup(cmd->args[0]);

        if (!head)
            head = cmd;
        if (prev)
            prev->next = cmd;
        prev = cmd;

        if (parser && parser->type == TOKEN_PIPE)
            parser = parser->next;
    }

    return head;
}



t_command *create_command(t_parser *parser)
{
    if(cheak_token(parser))
    {
            t_command *command = create_parser(parser);
    // if (command)
    //     command->exit_code = 0;
        free_list(parser);
        return command;
    }
    return NULL;
}

// int main(void)
// {
//     char *input = "cat > EOF | echo hello";

//     char *sort_input = filter(input);
//     t_token *token = tokenize(sort_input);
//     check_arr_of_token(token);
//     t_parser *parser = create_list(token->token_arr);
//     t_command *command = create_parser(parser);
//     free_list(parser);
//     print_command(command);
//     free_token(token);
//     free_comand(command);
//     return 0;
// }
// int main(void)
// {
//     char *input = "cat'hello| ||| world'<<'hello world'>>|echo";

//     char *sort_input = filter(input);
//     t_token *token = tokenize(sort_input);
//     check_arr_of_token(token);
    
//     t_command *parser_list = (token->token_arr);
//     if (parser_list) {
//         print_list(parser_list);
//         // check_symbol(parser_list);
//         free_list(parser_list);
//     }
//     free(sort_input);
//     free_token(token);
//     return 0;
// }