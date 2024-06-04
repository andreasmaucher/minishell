/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* used for exiting the program and freeing the memory correctly */
void	*set_pt_to_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

/* helper function for deleting a token and freeing its memory */
void	delete_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (content == NULL)
	{
		return ;
	}
	if (token->str != NULL)
	{
		free(token->str);
	}
	free(token);
}

/*
free args element of command list
*/
void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		i++;
	}
	free(args);
}

void	delete_file(void *content)
{
	t_file	*file;

	file = (t_file *)content;
	if (content == NULL)
		return ;
	if (file->file_name != NULL)
		free_to_null(file->file_name);
	free (file);
}

void	free_out_file_list(t_list *out_file)
{
	t_list		*current;
	t_list		*next;

	current = out_file;
	while (current != NULL)
	{
		free(current->value);
		current->value = NULL;
		next = current->next;
		free(current);
		current = next;
	}
}
