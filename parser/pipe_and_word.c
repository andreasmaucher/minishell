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
modifies the attributes of the command node in the list that 
m.clist points to
*/
void	cmd_pipe(t_list **clist, bool *new_cmd)
{
	t_command	*tmp_cmd;

	tmp_cmd = (t_command *)(*clist)->value;
	tmp_cmd->before_pipe = true;
	*clist = (*clist)->next;
	tmp_cmd = (t_command *)(*clist)->value;
	tmp_cmd->after_pipe = true;
	*new_cmd = true;
}

/* 
checks if the token is a builtin command and assigns the respective type, 
if not the command path needs to be searched for during execution
*/
void	check_if_builtin(t_token *tmp_token, t_command *tmp_command)
{
	if (!ft_strcmp(tmp_token->str, "echo")
		|| !ft_strcmp(tmp_token->str, "cd")
		|| !ft_strcmp(tmp_token->str, "pwd")
		|| !ft_strcmp(tmp_token->str, "export")
		|| !ft_strcmp(tmp_token->str, "unset")
		|| !ft_strcmp(tmp_token->str, "env")
		|| !ft_strcmp(tmp_token->str, "exit"))
		tmp_command->type = BUILTIN;
	else
		tmp_command->type = PATH;
}

/* 
If token type != redirection args element is filled with the str element.
For redirections the str is saved as file name as a t_file struct member.
For this case the second if condition moves the token_list
one token forward to skip the str allocation. Regardless of token type this
function also moves the token list one node forward.
*/
void	cmd_word_move_forward(t_token **tmp_token, t_command *tmp_command,
			int *i, t_list **tmp_tlist)
{
	if ((*tmp_token)->type != REDIRECT_IN
		&& (*tmp_token)->type != REDIRECT_OUT
		&& (*tmp_token)->type != REDIRECT_APPEND
		&& (*tmp_token)->type != REDIRECT_HEREDOC)
	{
		tmp_command->args[(*i)] = (*tmp_token)->str;
		(*i)++;
	}
	if ((*tmp_token)->type == REDIRECT_OUT
		|| (*tmp_token)->type == REDIRECT_APPEND
		|| (*tmp_token)->type == REDIRECT_IN
		|| (*tmp_token)->type == REDIRECT_HEREDOC)
		*tmp_tlist = (*tmp_tlist)->next;
	*tmp_tlist = (*tmp_tlist)->next;
	if (*tmp_tlist != NULL)
		*tmp_token = (t_token *)(*tmp_tlist)->value;
}

/* counts the total amount of token in the token list returned by the lexer;
this result is used to determine the size of **args in the command structure */
int	token_count_tlist(t_list *tlist)
{
	t_list	*tmp_head;
	t_token	*tmp_token;
	int		len;

	tmp_head = tlist;
	len = 0;
	tmp_token = NULL;
	while (tmp_head != NULL) 
	{
		tmp_token = (t_token *) tmp_head->value;
		if (tmp_token->type == PIPE)
			break ;
		if (tmp_token->type != REDIRECT_IN && tmp_token->type != REDIRECT_OUT 
			&& tmp_token->type != REDIRECT_APPEND
			&& tmp_token->type != REDIRECT_HEREDOC)
			len++;
		if (tmp_token->type == REDIRECT_IN || tmp_token->type == REDIRECT_OUT
			|| tmp_token->type == REDIRECT_APPEND
			|| tmp_token->type == REDIRECT_HEREDOC)
			tmp_head = tmp_head->next;
		tmp_head = tmp_head->next;
	}
	return (len);
}

/*
this function allocates memory for the args within the command structure;
the number of args depends on the length of the token list (tlist);
arguments are added from the token list until a pipe is encountered or
the end of the list is reached;
*/
void	cmd_word(t_list **tlist, t_list *clist, bool *new_cmd)
{
	t_token		*tmp_token;
	t_command	*tmp_command;
	int			i;
	int			tlist_len;
	t_list		*tmp_tlist;

	i = 0;
	if (*new_cmd == true)
	{
		tmp_tlist = NULL;
		tmp_tlist = *tlist;
		tmp_command = (t_command *) clist->value;
		tmp_token = (t_token *) tmp_tlist->value;
		check_if_builtin(tmp_token, tmp_command);
		tlist_len = token_count_tlist(tmp_tlist);
		tmp_command->args = malloc(sizeof(char *) * (tlist_len + 1));
		if (!tmp_command->args)
			return ;
		i = 0;
		while (tmp_token->type != PIPE && i <= tlist_len && tmp_tlist != NULL)
			cmd_word_move_forward(&tmp_token, tmp_command, &i, &tmp_tlist);
		tmp_command->args[i] = NULL;
	}
	*new_cmd = false;
}
