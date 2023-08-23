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
/* int *check_tokens(char *line)
{
	int i;
	char *str;

	i = 0;
	while (line[i])
	{
		str = check_for_word(line);
		i++;
	}
	return(str);
} */

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
