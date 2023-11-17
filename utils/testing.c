/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:13:42 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

/* used to print out the token types (only for testing purposes) */
const char *token_type_names_parser[] = 
{
    "NOT SET",
	"WORD",
	"WHITESPACE",
    "PIPE",
    "REDIRECT_HEREDOC",
    "REDIRECT_APPEND",
    "REDIRECT_IN",
    "REDIRECT_OUT",
};

const char *command_name_types_parser[] = 
{
	"BUILTIN",
	"PATH",
};

void print_command_list(t_list *clist) 
{
    t_list *current = clist;
    int i = 0;
    int command_count = 0;

    

    printf("\n--- Command List after Parser ---\n");
    while (current != NULL) 
    {
        t_command *cmd = (t_command *)current->value;
        printf("[%d]\n", command_count++);
        printf("Command Type: %s\n", command_name_types_parser[cmd->type]);
        printf("Before Pipe: %s\n", cmd->before_pipe ? "true" : "false");
        printf("After Pipe: %s\n", cmd->after_pipe ? "true" : "false");

        if (cmd->input_redir_type == 4 || cmd->input_redir_type == 6)
        {
            printf("--- Input Redirections or Heredoc ---\n");
            printf("Input redirection type: %s\n", token_type_names_parser[cmd->input_redir_type]);
            printf("File Descriptor (fd): %d\n", cmd->in_redirects.fd);
            //printf("File Name: %s\n", cmd->in_redirects.file_name != NULL ? cmd->in_redirects.file_name : "None");
            printf("Stop Heredoc: %s\n", cmd->in_redirects.stop_heredoc != NULL ? cmd->in_redirects.stop_heredoc : "None");
            printf("New Heredoc File: %s\n", cmd->in_redirects.new_heredoc_file != NULL ? cmd->in_redirects.new_heredoc_file : "None");
        }
       
        if (cmd->output_redir_type == 5 || cmd->output_redir_type == 7)
        {
            printf("--- Out Redirection --- \n");
            printf("Output redirection type: %s\n", token_type_names_parser[cmd->output_redir_type]);
            printf("File Descriptor (fd): %d\n", cmd->out_redirects.fd);
            printf("File Name: %s\n", cmd->out_redirects.file_name != NULL ? cmd->out_redirects.file_name : "None");
        }
		// while (cmd->out_file != NULL)
        // {
        //     printf("Out file name for redirect: %s\n", (char *)cmd->out_file->value);
        //     cmd->out_file = cmd->out_file->next;
        // }
        // while (cmd->in_file != NULL)
        // {
        //     printf("Out file name for redirect: %s\n", (char *)cmd->in_file->value);
        //     cmd->in_file = cmd->in_file->next;
        // }
        // i = 0;
        // while (cmd->args[i] != NULL)
        // {
        //     printf("Arguments: %s\n", cmd->args[i]);
        //     i++;
        // }
        // Move to the next node
        current = current->next;
        i++;
    }
}

/* used to print out the token types (only for testing purposes) */
const char *token_type_names[] = 
{
    "NOT SET",
	"WORD",
	"WHITESPACE",
    "PIPE",
    "REDIRECT_HEREDOC",
    "REDIRECT_APPEND",
    "REDIRECT_IN",
    "REDIRECT_OUT",
	"ENV",
	"ENV_FAIL",
};

/* only for testing purposes, prints a list */
void	printlist(t_list *head)
{
	t_list	*temporary;
	t_token *token;
	int i;

	i = 0;
	temporary = head;
    printf("\n--- Token List after Lexer ---\n");
	while (temporary != NULL)
	{
		token = (t_token *)temporary->value;
		printf("[%d]: %s type: %s\n", i, token->str, token_type_names[token->type]); // casted to char since in the first test we want to print a word
		temporary = temporary->next;
		i++;
	}
}