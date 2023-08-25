/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:16:54 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:16:56 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_token
{
	int		type;
	char	*str;
}	t_token;

//token type
typedef enum type
{
	WORD,
	WHITESPACE,
}	t_type;

/*parser_utils*/
char	*ft_substr(char const *s, unsigned int start, size_t len);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_back(t_list **lst, t_list *new);


#endif
