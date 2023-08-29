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

t_token *add_token_type_and_str(char *str_with_all_tokens, t_type token_type)
{
	t_token *token;

	token = malloc(sizeof(t_token)); //! where to free this?
	if (!token)
		return(NULL);
	token->str = str_with_all_tokens;
	token->type = token_type;
	printf("token_type %d\n", token->type);
	return(token);
}

// add multiple checks for all kind of delimiters e.g. parameter, quotes, whitespaces
void check_for_tokens(t_data m) //! could be void
{
	int 	i;
	char 	*str_with_all_tokens;

	i = 0;
	m.list = NULL;
	while (m.line[i])
	{
		if (m.line[i] == '|')
			str_with_all_tokens = pipe_token(&i, &m.token_type);
		else if (m.line[i] == '<' || m.line[i] == '>')
			str_with_all_tokens = redirection_token(m.line, &i, &m.token_type);
		else
			str_with_all_tokens = check_for_word_token(m.line, &i, &m.token_type);
		printf("str_with_all_tokens: %s\n", str_with_all_tokens);
		//m.list = add_token_to_list(&m.list, str_with_all_tokens, m.token_type);
		m.list = ft_lstadd_back(&m.list, ft_lstnew(str_with_all_tokens));
		i++; //! this needs to go!
	}
	// here we could go through the full list & remove whitespace & merge or split words
}

/* to free up all memory in the end*/
void	freememory(t_data m)
{
	t_list	*delete;

	free(m.line);
	while (lst_size(m.list) > 0)
	{
		delete = m.list;
		m.list = m.list->next;
		free(delete);
	}
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
			freememory(m);
            exit(1);
        }
		check_for_tokens(m);
		add_history(m.line);
		free(m.line); // memory automatically allocated by readline function
	}
}
