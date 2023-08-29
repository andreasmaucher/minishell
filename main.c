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

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>

/* LEXER: check for various tokens: 
	1. redirections < >
	2. $
	3. '\'
	4. whitespace
	5. | 
	6. check if it's a character string / word
*/

t_token *add_token_type_and_str(char *str_with_all_tokens, t_type token_type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return(NULL);
	token->str = str_with_all_tokens;
	token->type = token_type;
	return(token);
}

/* determine the size of a list */
int	lst_size(t_list *head)
{
	int		lsize;
	t_list	*current_node_pos;

	lsize = 0;
	current_node_pos = head;
	while (current_node_pos != NULL)
	{
		current_node_pos = current_node_pos->next;
		lsize++;
	}
	return (lsize);
}

t_list	*return_tail_value(t_list *head)
{
	t_list	*current_node_pos;
	int		lsize;

	current_node_pos = head;
	lsize = lst_size(head);
	while (lsize > 1)
	{
		current_node_pos = current_node_pos->next;
		lsize--;
	}
	return (current_node_pos);
}

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

t_list	*insert_at_tail(t_list *head, t_list *new_value)
{
	t_list	*new_node;
	t_list	*current;

	new_node = create_new_node(new_value);
	current = head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	return (head);
}

t_list *add_token_to_list(t_list **token_list, char *str_with_all_tokens, t_type token_type)
{
	t_list *new_node;
	t_token *data;

	data = add_token_type_and_str(str_with_all_tokens, token_type);
	if (!data)
		return (NULL);
	new_node = create_new_node(data);
	insert_at_tail(*token_list, new_node);
	return (*token_list);
}

bool	check_for_metacharacter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<' || c == '>')
		return (true);
	else
		return (false);
}

char *check_for_word_token(char *line, int *i, t_type *token_type)
{
	int token_start;

	token_start = *i;
	while (check_for_metacharacter(line[*i]) == false && line[*i] != '$')
		(*i)++;
	*token_type = WORD;
	return(ft_substr(line, token_start, *i - token_start));
}

// add multiple checks for all kind of delimiters e.g. pipes, redirects
t_list *check_for_tokens(char *line)
{
	int 	i;
	char 	*str_with_all_tokens;
	t_list 	*token_list;
	t_type 	token_type;

	i = 0;
	token_list = NULL;
	while (line && line[i]) // first check ensures that line points to a valid memory address
	{
		str_with_all_tokens = check_for_word_token(line, &i, &token_type);
		token_list = add_token_to_list(&token_list, str_with_all_tokens, token_type);
	}
	// here we could go through the full list & remove whitespace & merge or split words
	return(token_list);
}

int main(int ac, char **av)
{
	char *line;
	//t_data data;

	(void) ac;
	(void) av;
	while(1)
	{
		line = readline("Myshell: ");
		check_for_tokens(line); // where do we store this?
		//add_history(line);
		printf("%s\n", line);
		free(line); // memory automatically allocated by readline function
	}
}
