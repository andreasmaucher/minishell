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

/*
- PIPE can not be the first or last token
- previous / next token after / before PIPE can not be PIPE, meaning no
 consecutive pipes
*/
bool	check_parser_pipes(t_list *tlist)
{
	t_list		*current;
	t_list		*previous;
	bool		check;

	current = tlist;
	previous = NULL;
	check = true;
	while (current != NULL)
	{
		if (((t_token *)current->value)->type == PIPE)
		{
			if (previous == NULL || current->next == NULL
				|| ((t_token *)previous->value)->type == PIPE
				|| ((t_token *)current->next->value)->type == PIPE)
				check = false;
		}
		previous = current;
		current = current->next;
	}
	return (check);
}

/*
REDIRECTS can not be the last token, at least one token needs to follow
and this token needs to be of type WORD
*/
bool	check_parser_redirections(t_list *tlist)
{
	t_list	*current;
	bool	check;

	current = tlist;
	check = true;
	while (current != NULL)
	{
		if ((((t_token *)current->value)->type == REDIRECT_HEREDOC)
			|| (((t_token *)current->value)->type == REDIRECT_APPEND)
			|| (((t_token *)current->value)->type == REDIRECT_IN)
			|| (((t_token *)current->value)->type == REDIRECT_OUT))
		{
			if (current->next == NULL
				|| ((t_token *)current->next->value)->type != WORD)
				check = false;
		}
		current = current->next;
	}
	return (check);
}

/* 
checks if all conditions for a valid parser input are fullfilled:
*/
bool	check_parser_input(t_list *tlist)
{
	if (check_parser_redirections(tlist) == false
		|| check_parser_pipes(tlist) == false)
	{
		printf("Invalid parser syntax.");
		return (false);
	}
	return (true);
}
