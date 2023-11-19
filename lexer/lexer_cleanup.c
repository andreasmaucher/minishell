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

#include "../minishell.h"

/* used for joining two consecutive word tokens; the function takes in a 
pointer to the head of thelist and two pointers to the current node and
the previous node;if the token type of the previous node is WORD the string
contents of the two consecutive tokens are joined into one and one token
is freed */
void	join_str_and_del_old_node(t_list *tlist, t_list *current_node,
		t_list *previous_node)
{
	char		*new_joined_str;
	t_token		*previous_token;

	previous_token = previous_node->value;
	if (previous_token->type == WORD)
	{
		if (previous_token->str != NULL
			&& ((t_token *)current_node->value)->str != NULL)
		{
			new_joined_str = ft_strjoin(previous_token->str,
					((t_token *)current_node->value)->str); 
			free(((t_token *)previous_node->value)->str);
			((t_token *)previous_node->value)->str = new_joined_str;
			ft_lstremove(&tlist, current_node, delete_token);
		}
	}
}

/* function that merges two consecutive word tokens (without spaces in between!)
e.g. l"s"; otherwise the parser would process this as two separate tokens */
t_list	*merge_tokens(t_list **tlist, t_list *current_node)
{
	t_list		*previous_node;
	t_token		*current_token;

	previous_node = NULL;
	current_token = current_node->value;
	if (current_token->type == WORD)
	{
		previous_node = find_previous_node(*tlist, current_node);
		if (previous_node != NULL)
			join_str_and_del_old_node(*tlist, current_node, previous_node);
	}
	return (*tlist);
}

/*
delete all whitespace tokens at the end of the lexer
*/
t_list	*delete_whitespace(t_list **tlist, t_list *current_node)
{
	t_token	*tmp_token;

	tmp_token = current_node->value;
	if (tmp_token->type == WHITESPACE)
		ft_lstremove(tlist, current_node, delete_token);
	return (*tlist);
}

/*
applies a function to every node of a list
*/
t_list	*apply_function_to_list(t_list **tlist, t_list *(*f)
		(t_list **tlist, t_list *current_node))
{
	t_list	*current_node;
	t_list	*next;

	current_node = *tlist;
	while (current_node)
	{
		next = current_node->next;
		*tlist = (*f)(tlist, current_node);
		current_node = next;
	}
	return (*tlist);
}

t_list	*cleanup_token_list(t_list *tlist)
{
	tlist = apply_function_to_list(&tlist, merge_tokens);
	tlist = apply_function_to_list(&tlist, delete_whitespace);
	return (tlist);
}
