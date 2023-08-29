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

/* LEXER: check for various tokens: 
	1. redirections < >
	2. $
	3. '\'
	4. whitespace
	5. | 
	6. check if it's a character string / word
*/

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
		printf("list[%d]: %s\n", i, token->str); // casted to char since in the first test we want to print a word
		temporary = temporary->next;
		i++;
	}
	printf("\n");
}

t_token *add_token_type_and_str(char *str_with_all_tokens, t_type token_type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return(NULL);
	token->str = str_with_all_tokens;
	token->type = token_type;
	printf("token_type %d\n", token->type);
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

t_list *add_token_to_list(t_list **token_list, char *str_with_all_tokens, t_type token_type)
{
	t_list *new_node;
	t_token *data;

	data = add_token_type_and_str(str_with_all_tokens, token_type);
	if (!data)
		return (NULL);
	new_node = NULL;
	new_node = create_new_node(data);
	if (!*token_list)
		*token_list = new_node;
	else
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

//checks if c is a double or single quote
bool	check_for_quotes(char c)
{
	if (c == '"' || c == '\'')
		return (true);
	else
		return (false);
}

char *redirection_token(char *line, int *i, t_type *token_type)
{
	(*i)++;
	if (line[*i] == '<' && line[*i++] == '<')
	{
		*token_type = REDIRECT_HEREDOC;
		(*i)++;
		return (ft_strdup("<<"));
	}
	else if (line[*i] == '>' && line[*i++] == '>')
	{
		*token_type = REDIRECT_APPEND;
		(*i)++;
		return (ft_strdup(">>"));
	}
	else if (line[*i] == '>') //! not detected correctly
	{
		*token_type = REDIRECT_OUT;
		return (ft_strdup(">"));
	}
	else
	{
		*token_type = REDIRECT_IN;
		return (ft_strdup("<"));
	}
}

char *pipe_token(int *i, t_type *token_type)
{
	*token_type = PIPE;
	(*i)++;
	return(ft_strdup("|"));
}

char *check_for_word_token(char *line, int *i, t_type *token_type)
{
	int token_start;

	token_start = *i;
	while (check_for_metacharacter(line[*i]) == false && check_for_quotes(line[*i]) == false && line[*i] != '$')
		(*i)++;
	*token_type = WORD;
	return(ft_substr(line, token_start, *i - token_start));
}

// add multiple checks for all kind of delimiters e.g. parameter, quotes, whitespaces
t_list *check_for_tokens(t_data m)
{
	int 	i;
	char 	*str_with_all_tokens;
	t_type 	token_type;

	i = 0;
	m.list = NULL;
	while (m.line[i])
	{
		if (m.line[i] == '|')
			str_with_all_tokens = pipe_token(&i, &token_type);
		else if (m.line[i] == '<' || m.line[i] == '>')
			str_with_all_tokens = redirection_token(m.line, &i, &token_type);
		else
			str_with_all_tokens = check_for_word_token(m.line, &i, &token_type);
		//printf("str_with_all_tokens: %s\n", str_with_all_tokens);
		m.list = add_token_to_list(&m.list, str_with_all_tokens, token_type);
		i++; //! this needs to go!
	}
	// here we could go through the full list & remove whitespace & merge or split words
	return(m.list);
}


int main(void)
{
	t_data m;

	while(1)
	{
		m.line = readline("Myshell: ");
		// Check for end-of-input or Ctrl+D
        if (m.line == NULL || strcmp(m.line, "exit") == 0) {
            printf("\nExiting...\n");
            free(m.line);
            exit(1);
        }
		printlist(check_for_tokens(m)); //! where do we store this?
		add_history(m.line);
		free(m.line); // memory automatically allocated by readline function
	}
}
