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
#include <stdbool.h>
#include <string.h>

//token type
typedef enum type
{
	WORD,
	WHITESPACE,
    PIPE,
    REDIRECT_HEREDOC,
    REDIRECT_APPEND,
    REDIRECT_IN,
    REDIRECT_OUT,
}	t_type;
/* typedef struct s_list
{
	void *value; //value needs to be void since we want to be able to store multiple different types
	struct s_list	*next;
}	t_list; */

typedef struct s_list
 {
	int				content;
	int				index;
	int				local_index;
	struct s_list	*next;
}	t_list;

//main struct!
typedef struct s_data
{
    int tokens;
    char *line;
    t_list *list;
    t_type token_type;
}   t_data;

typedef struct s_token
{
	int		type;
	char	*str;
}	t_token;


//main
t_token *add_token_type_and_str(char *str_with_all_tokens, t_type token_type);

//parser_utils
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);

//list operators
/* void	printlist(t_list *head);
int	lst_size(t_list *head);
t_list	*return_tail_value(t_list *head);
t_list	*create_new_node(void *value);
void	insert_at_tail(t_list *head, t_list *new_value);
t_list *add_token_to_list(t_list **token_list, char *str_with_all_tokens, t_type token_type);
 */
t_list	*ft_lstnew(int content);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	printlist(t_list *head);
int	ft_lstsize(t_list *lst);
void	free_list(t_list	*head);

//token checks
char *redirection_token(char *line, int *i, t_type *token_type);
char *pipe_token(int *i, t_type *token_type);
char *check_for_word_token(char *line, int *i, t_type *token_type);

//lexer utils
bool	check_for_metacharacter(char c);
bool	check_for_quotes(char c);

#endif
