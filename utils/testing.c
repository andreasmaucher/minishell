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

//used to print out the token types (only for testing purposes)
const char *token_type_names_parser[] = 
{
	"WORD",
	"WHITESPACE",
    "PIPE",
    "REDIRECT_HEREDOC",
    "REDIRECT_APPEND",
    "REDIRECT_IN",
    "REDIRECT_OUT",
	"DOUBLE_QUOTES",
    "SINGLE_QUOTES"
};

void print_command_list(t_list *clist) 
{
    t_list *current = clist;

    while (current != NULL) {
        t_command *cmd = (t_command *)current->value;

        printf("Command Type: %d\n", cmd->type);
        printf("Before Pipe: %s\n", cmd->before_pipe ? "true" : "false");
        printf("After Pipe: %s\n", cmd->after_pipe ? "true" : "false");
        printf("Input redir or heredoc: %s\n", token_type_names_parser[cmd->input_redir_or_heredoc]);
        //printf("list[%d]: %s type: %s\n", i, token->str, token_type_names[token->type]);
        
        // Check and print the inred_file and outred_file attributes
        //printf("In Redir File: %s\n", cmd->inred_file != NULL ? (char *)(cmd->inred_file) : "None");
        /* t_list *inred_list = cmd->inred_file;
        while (inred_list != NULL) {
            printf("In Redir File: %s\n", (char *)(inred_list->value));
            inred_list = inred_list->next;
        } */
        t_list *inred_list = cmd->inred_file;
        while (inred_list != NULL)
    {
        t_file *file = (t_file *)inred_list->value;
            printf("--- Input Redirections or Heredoc ---\n");
            printf("File Descriptor (fd): %d\n", file->fd);
            printf("Text to File: %s\n", file->text_to_file != NULL ? file->text_to_file : "None");
            printf("Stop Heredoc: %s\n", file->stop_heredoc != NULL ? file->stop_heredoc : "None");
            printf("Redirection Type: %s\n", token_type_names_parser[file->redirection_type]);
            printf("New Heredoc File: %s\n", file->new_heredoc_file != NULL ? file->new_heredoc_file : "None");
            inred_list = inred_list->next;
    }
        printf("Out Redir Type: %s\n", token_type_names_parser[cmd->out_redir_type]);
        //printf("Out Redir File: %s\n", cmd->outred_file != NULL ? (char *)(cmd->outred_file) : "None");
        t_list *outredir_list = cmd->outred_file;
        while (outredir_list != NULL) {
            t_file *file = (t_file *)outredir_list->value;
            printf("--- Out Redirection --- \n");
            printf("File Descriptor (fd): %d\n", file->fd);
            printf("Text to File: %s\n", file->text_to_file != NULL ? file->text_to_file : "None");
            printf("Stop Heredoc: %s\n", file->stop_heredoc != NULL ? file->stop_heredoc : "None");
            printf("Redirection Type: %s\n", token_type_names_parser[file->redirection_type]);
            printf("New Heredoc File: %s\n", file->new_heredoc_file != NULL ? file->new_heredoc_file : "None");
            outredir_list = outredir_list->next;
        }
        ///printf("Arguments: %s\n", (char *)cmd->arguments);
        t_list *arguments_list = cmd->arguments;
        while (arguments_list != NULL) {
            printf("Argument: %s\n", (char *)(arguments_list->value));
            arguments_list = arguments_list->next;
        }
        //printf("Arguments: %s\n", cmd->arguments != NULL ? (char *)(cmd->arguments) : "None");

        // Move to the next node
        current = current->next;
    }
}

//used to print out the token types (only for testing purposes)
const char *token_type_names[] = 
{
	"WORD",
	"WHITESPACE",
    "PIPE",
    "REDIRECT_HEREDOC",
    "REDIRECT_APPEND",
    "REDIRECT_IN",
    "REDIRECT_OUT",
	"DOUBLE_QUOTES",
    "SINGLE_QUOTES",
	"ENV",
	"ENV_FAIL",
};

//only for testing purposes, prints a list
void	printlist(t_list *head)
{
	t_list	*temporary;
	t_token *token;
	int i;

	i = 0;
	temporary = head;
	while (temporary != NULL)
	{
		token = (t_token *)temporary->value;
		printf("list[%d]: %s type: %s\n", i, token->str, token_type_names[token->type]); // casted to char since in the first test we want to print a word
		temporary = temporary->next;
		i++;
	}
}