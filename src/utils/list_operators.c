/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_operators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:19:57 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* creates a new node (that is to be added to a list in subsequent function) */
t_list	*create_new_node(void *value)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->value = value;
	newnode->next = NULL;
	return (newnode);
}

/* creates a new node for in_filenames(that is to be added to a 
list in subsequent function) */
t_list	*create_new_filename_node(void *value, char *eof)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->value = value;
	newnode->eof = eof;
	newnode->is_heredoc = 0;
	newnode->next = NULL;
	return (newnode);
}

/* creates a new node for in_filenames(that is to be added to 
a list in subsequent function) */
t_list	*create_new_append_node(void *value)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->value = value;
	newnode->is_append = 0;
	newnode->next = NULL;
	return (newnode);
}

/* inserts a node at the end of a list */
void	insert_at_tail(t_list *head, t_list *new_value)
{
	t_list	*current;

	if (head == 0 || new_value == 0)
		return ;
	current = head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_value;
}

/*
adds a new node to a list; in case the list is empty, 
the new node becomes the
head, else it is added at the end of the list
*/
t_list	*add_token_to_list(t_list **token_list, char *token_str,
			t_type token_type)
{
	t_list	*new_node;
	t_token	*data;

	data = add_token_type_and_str(token_str, token_type);
	if (!data)
		return (NULL);
	new_node = create_new_node(data);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
	return (*token_list);
}
