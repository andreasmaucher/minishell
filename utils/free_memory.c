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

/* used for exiting the program and freeing the memory correctly */
void	*ft_free_set_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

static void	file_del(void *arg)
{
	t_file	*file;

	file = (t_file *)arg;
	if (file->redirection_type == REDIRECT_HEREDOC)
		unlink(file->new_heredoc_file);
	//! also free text to file
	file->new_heredoc_file = ft_free_set_null(file->new_heredoc_file);
	file->stop_heredoc = ft_free_set_null(file->stop_heredoc);
	file = ft_free_set_null(file);
}

/* helper function for deleting a token and freeing its memory */
void token_del(void *content) 
{
    if (content == NULL) {
        return;
    }

    t_token *token = (t_token *)content;
    if (token->str != NULL) {
        free(token->str); // Assuming the str field is dynamically allocated.
    }
    
    free(token); // Free the token structure itself.
}

void	command_del(void *arg)
{
	t_command	*command;

	command = (t_command *)arg;
	ft_lstclear(&command->arguments, free);
	ft_lstclear(&command->outred_file, file_del);
	ft_lstclear(&command->inred_file, file_del);
	command = ft_free_set_null(command);
}

//! DO WE NEED TO REPLICATE BASH EXIT CODES?
/* to free up all memory in the end, including memory automatically allocated by readline function */
int	exit_shell(t_minishell m)
{
	if (m.line)
		m.line = ft_free_set_null(m.line);
	if (m.tlist)
		ft_lstclear(&m.tlist, token_del);
	if (m.clist)
		ft_lstclear(&m.clist, command_del);
	printf("\nExiting...\n");
	exit(1);
}