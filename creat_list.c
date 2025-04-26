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
	return (TOKEN_WORD);
}

char *delete_quotes(const char *s, char q)
{
    int len = ft_strlen(s);
    int i = 1;
    int j = 0;
    char *res;

    if (len < 2 || s[0] != q || s[len - 1] != q)
        return ft_strdup(s);

    res = malloc(len - 1);
    if (!res)
        return NULL;

    while (i < len - 1)
    {
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
t_parser *create_list(char **ss, t_env *my_env)
{
    int i = 0;
    t_parser *head = NULL;
    t_parser *current = NULL;
    t_parser *new_node;
    char *word;

    while (ss[i])
    {
        if(ss[i][0] == '$' )
        {
            char *key = get_parametr(ss[i]);
            t_env *my_key = find_key(my_env, key);
            word = ft_strdup(my_key->value);
            new_node = creat_node(word);
            free(key);
            free(word);
        }

        else if ((ss[i][0] == '\'' || ss[i][0] == '"') && ss[i][ft_strlen(ss[i]) - 1] == ss[i][0])
        {
            word = delete_quotes(ss[i], ss[i][0]);
            new_node = creat_node(word);
            if (ss[i][0] == '\'')
                new_node->single_quotes = 1;
            else
                new_node->double_quotes = 1; 
            free(word);
        }
        else
        {
            new_node = creat_node(ss[i]);
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
    printf("    file: %s\n", redir->file ? redir->file : "(null)");
    printf("    is_output: %s\n", redir->is_output ? "true" : "false");
    printf("    append: %s\n", redir->append ? "true" : "false");
    printf("    is_heredoc: %s\n", redir->is_heredoc ? "true" : "false");
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
        print_redirect(cmd->input);

        printf("Output redirect:\n");
        print_redirect(cmd->output);

        printf("\n---\n");
        cmd = cmd->next;
    }
}


void init_comand(t_command *comand)
{
    comand->args = NULL;
    comand->input = NULL;
    comand->next = NULL;
    comand->output = NULL;
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

        if (command->input)
        {
            if (command->input->file)
                free(command->input->file);
            free(command->input);
        }

        if (command->output)
        {
            if (command->output->file)
                free(command->output->file);
            free(command->output);
        }

        free(command);
        command = tmp;
    }
    free(command);
}



void init_redirin(char *file, t_token_type inp, t_command *cmd)
{
    if (!cmd->input)
    {
        cmd->input = malloc(sizeof(t_redirect));
        if (!cmd->input)
            return;
        cmd->input->file = NULL;
    }

    if(inp == TOKEN_REDIR_IN)
    {
        cmd->input->file = ft_strdup(file);
        cmd->input->is_heredoc = false;
        cmd->input->append = false;
        cmd->input->is_output = false;
    }
}

void init_heredok(t_token_type inp, t_command *cmd)
{
    if (!cmd->input)
    {
        cmd->input = malloc(sizeof(t_redirect));
        if (!cmd->input)
            return;
    }
    if (inp == TOKEN_REDIR_HEREDOC)
    {
        cmd->input->is_heredoc = true;
        cmd->input->append = false;
        cmd->input->is_output = false;
    }
}
void init_append(t_token_type inp, char *file, t_command *cmd)
{
    if (!cmd->input)
    {
        cmd->input = malloc(sizeof(t_redirect));
        if (!cmd->input)
            return;
    }

    cmd->input->file = ft_strdup(file);

    if(inp == TOKEN_REDIR_IN)
    {
        cmd->input->is_heredoc = false;
        cmd->input->append = true;
        cmd->input->is_output = false;
    }
}
void init_redir_out(t_token_type inp, char *file, t_command *cmd)
{
        if (!cmd->input)
        {
            cmd->input = malloc(sizeof(t_redirect));
            if (!cmd->input)
                return;
        }
    
        cmd->input->file = ft_strdup(file);
    
        if(inp == TOKEN_REDIR_IN)
        {
            cmd->input->is_heredoc = false;
            cmd->input->append = true;
            cmd->input->is_output = false;
        }
   
}

t_command *create_parser(t_parser *parser)
{
    t_command *head = NULL;
    t_command *prev = NULL;
    t_parser *tmp = parser;
    int i = 0;

    while (tmp)
    {
        i = 0;
        while (tmp && tmp->type != TOKEN_PIPE)
        {
            i++;
            tmp = tmp->next;
        }

        t_command *cmd = malloc(sizeof(t_command));
        if (!cmd)
            return NULL;
        init_comand(cmd);
        cmd->args = malloc(sizeof(char *) * (i + 1));
        if (!cmd->args)
            return NULL;

        int j = 0;
        cmd->command = ft_strdup(parser->word);
        while (parser && parser->type != TOKEN_PIPE)
        {
            cmd->args[j] = ft_strdup(parser->word);
            if(parser->type == TOKEN_REDIR_HEREDOC)
                init_heredok(parser->type, cmd);
            else if(parser->type == TOKEN_REDIR_APPEND)
                init_append(parser->type, parser->next->word, cmd);
            else if(parser->type == TOKEN_REDIR_OUT)
                init_redir_out(parser->type, parser->next->word, cmd);
            else if(parser->next && (parser->next->type == TOKEN_REDIR_IN))
                init_redirin(parser->word, parser->next->type, cmd);
            j++;
            parser = parser->next;
        }
        cmd->args[j] = NULL;

        if (!head)
            head = cmd;
        if (prev)
            prev->next = cmd;
        prev = cmd;

        if (parser)
            parser = parser->next;
        if (tmp)
            tmp = tmp->next;
    }
    free(tmp);
    return head;
}


t_command *create_command(t_parser *parser)
{
    t_command *command = create_parser(parser);

    free_list(parser);
    return command;
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