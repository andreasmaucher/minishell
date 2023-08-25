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

t_token *add_token_type(char *str, t_type type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return(NULL);
	token->str = str;
	token->type = type;
	return(token);
}

t_list *add_token_to_list(t_list **token_list, char *str, t_type type)
{
	t_list *new_node;
	t_token *data;

	data = add_token_type(str, type);
	if (!data)
		return (NULL);
	new_node = ft_lstnew(data);
	ft_lstadd_back(token_list, new_node);
	return (*token_list);
}

char *check_for_word_token(char *line, int i, t_type type)
{
	int token_start;

	token_start = i;
	while (line[i] && (line[i] != 't' || line[i] != '\n' || line[i] != '|' ||
			 line[i] != '<' || line[i] != '>'))
		i++;
	type = WORD;
	return(ft_substr(line, token_start, i - token_start));
}

/* add multiple checks for all kind of delimiters e.g. pipes, redirects */
int *check_tokens(char *line)
{
	int 	i;
	char 	*str;
	t_list 	*token_list;
	t_type 	type;

	i = 0;
	while (line[i])
	{
		str = check_for_word_token(line, i, &type);
		token_list = add_token_to_list(&token_list, str, type);
		i++;
	}
	return(str);
}

int main(int ac, char **av)
{
	char *line;

	(void) ac;
	(void) av;
	while(1)
	{
		line = readline("prompt: ");
		add_history(line);
		printf("%s", line);
		free(line); // memory automatically allocated by readline function
	}
}

//parse
