/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_token_to_command_list(t_list **token_list, char *token_info)
{
	t_list	*new_node;

	new_node = create_new_node(token_info);
	if (!*token_list)
		*token_list = new_node;
	else
		insert_at_tail(*token_list, new_node);
}

/*
function that adds attributes to the command list (m.clist);
- assign the value of a node in the token list to tmp_token
- depending on it's type execute a subsequent function to add the attributes
to the command list
*/
static void	add_attributes_to_command_list(t_minishell m)
{
	t_list	*tmp_tlist;
	t_token	*tmp_token;
	bool	new_cmd;

	new_cmd = true;
	tmp_tlist = m.tlist;
	while (tmp_tlist != NULL)
	{
		tmp_token = (t_token *)tmp_tlist->value;
		if (tmp_token->type == PIPE)
			cmd_pipe(&m.clist, &new_cmd);
		else if (tmp_token->type == REDIRECT_IN
			|| tmp_token->type == REDIRECT_HEREDOC)
			cmd_input_redirection(&tmp_tlist, m.clist);
		else if (tmp_token->type == WORD)
			cmd_word(&tmp_tlist, m.clist, &new_cmd);
		else if (tmp_token->type == REDIRECT_OUT
			|| tmp_token->type == REDIRECT_APPEND)
			cmd_output_redirection(&tmp_tlist, m.clist);
		tmp_tlist = tmp_tlist->next;
	}
}

/* 
check if the line is emtpy, meaning it only consists of whitespace
*/
bool	check_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

/* 
if the input fullfills the parser syntax the command list clist
is created anf dilled with elements;
if the line consists only of whitespace no error message is printed
For testing: print_command_list(m.clist);
*/
t_list	*parser(t_minishell m)
{
	if (check_parser_input(m.tlist) == true && m.tlist != NULL)
	{
		m.clist = setup_command_list(&m.clist, m.tlist);
		if (m.clist == NULL)
			return (NULL);
		add_attributes_to_command_list(m);
		return (m.clist);
	}
	else
	{
		if (errno == 0)
		{
			m.status_code = 2;
			if (check_empty_line(m.line) == true)
				return (NULL);
			printf("syntax error near unexpected token\n");
			return (NULL);
		}
	}
	return (NULL);
}
