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

/* m struct needs initialization
fix token type, add it to m struct
add additional functions
make sure the loop does not stop and run smoothly and that i can take out i++
free all memory
put functions into separate files */

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
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		*token_type = REDIRECT_HEREDOC;
		(*i) += 2;
		return (ft_strdup("<<"));
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		*token_type = REDIRECT_APPEND;
		(*i) += 2;
		return (ft_strdup(">>"));
	}
	else if (line[*i] == '>') //! not detected correctly
	{
		*token_type = REDIRECT_OUT;
		(*i) += 1;
		return (ft_strdup(">"));
	}
	else
	{
		*token_type = REDIRECT_IN;
		(*i) += 1;
		return (ft_strdup("<"));
	}
}

char *pipe_token(int *i, t_type *token_type)
{
	*token_type = PIPE;
	(*i)++;
	return(ft_strdup("|"));
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
    "REDIRECT_OUT"
};

char *check_for_word_token(char *line, int *i, t_type *token_type)
{
	int token_start;

	token_start = *i;
	while (check_for_metacharacter(line[*i]) == false && check_for_quotes(line[*i]) == false && line[*i] != '$' && line[*i])
		(*i)++;
	*token_type = WORD;
	return(ft_substr(line, token_start, *i - token_start));
}

// add multiple checks for all kind of delimiters e.g. parameter, quotes, whitespaces
t_list *split_line_into_tokens(t_data m)
{
	int 	i;

	i = 0;
	m.list = NULL;
	while (m.line[i])
	{
		if (m.line[i] == '|')
			m.str_with_all_tokens = pipe_token(&i, &m.token_type);
		else if (m.line[i] == '<' || m.line[i] == '>')
			m.str_with_all_tokens = redirection_token(m.line, &i, &m.token_type);
	/* 	else if (m.line[i] == '$')
			str_with_all_tokens = env_token(m.line, &i, &token_type);
		else if (whitespace(m.line[i]) == true)
			str_with_all_tokens = whitespace_token(m.line, &i, &token_type); */
		/* else if (m.line[i] == '\'' || m.line[i] == '\"')
			str_with_all_tokens = single_or_double_quotes_token(m.line, &i, &token_type); */
		else
			m.str_with_all_tokens = check_for_word_token(m.line, &i, &m.token_type);
		m.list = add_token_to_list(&m.list, m.str_with_all_tokens, m.token_type);
		printf("TT: %s\n", token_type_names[m.token_type]);
		i++; //! this needs to go!
	}
	// here we could go through the full list & remove whitespace & merge or split words
	return(m.list);
}

//! SEGFAULTS AT EXIT
/* to free up all memory in the end, including memory automatically allocated by readline function */
void	freememory(t_data m)
{
	t_list	*delete;

	while (lst_size(m.list) > 0)
	{
		delete = m.list;
		m.list = m.list->next;
		free(delete);
	}
	free(m.line);
}

int main(void)
{
	t_data m;

	while(1)
	{
		m.line = readline("Myshell: ");
		// Check for end-of-input or Ctrl+D
        if (m.line == NULL || ft_strcmp(m.line, "exit") == 0) {
            printf("\nExiting...\n");
			freememory(m);
            exit(1);
        }
		printlist(split_line_into_tokens(m)); //! where do we store this?
		add_history(m.line);
	}
}
