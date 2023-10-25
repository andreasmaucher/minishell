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
adds a new node to a list; in case the list is empty, the new node becomes
the head, else it is added at the end of the list
*/
t_list	*create_command_list(t_list **clist, t_command *tmp_cmd)
{
	t_list	*new_node;

	new_node = create_new_node(tmp_cmd);
	if (!*clist)
		*clist = new_node;
	else
		insert_at_tail(*clist, new_node);
	return (*clist);
}

t_command	*ft_create_cmd(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = PATH;
	cmd->before_pipe = false;
	cmd->after_pipe = false;
	cmd->input_redir_type = 0;
	cmd->output_redir_type = 0;
	cmd->args = NULL;
	cmd->out_redirects.fd = -1;
	cmd->out_redirects.file_name = NULL;
	cmd->out_redirects.stop_heredoc = NULL;
	cmd->out_redirects.new_heredoc_file = NULL;
	cmd->in_redirects.fd = -1;
	cmd->in_redirects.file_name = NULL;
	cmd->in_redirects.stop_heredoc = NULL;
	cmd->in_redirects.new_heredoc_file = NULL;
	return (cmd);
}

/*
counts the amount of commands (len) in a token list, determined by the
amount of PIPES;iteration by 1 at the end to account for the fact, that
a PIPE is always splitting two parts
*/
int	command_count(t_list *tlist)
{
	t_list	*tmp_head;
	t_token	*token;
	int		len;

	tmp_head = tlist;
	len = 0;
	while (tmp_head != NULL) 
	{
		token = (t_token *)tmp_head->value;
		if (token->type == PIPE)
			len++;
		tmp_head = tmp_head->next;
	}
	len++;
	return (len);
}

/*
initialization of clist to NULL, to ensure that it starts as
an empty list
*/
t_list	*setup_command_list(t_list **clist, t_list *tlist)
{
	int			len;
	t_command	*tmp_cmd;

	*clist = NULL;
	len = command_count(tlist);
	while (len--)
	{
		tmp_cmd = ft_create_cmd();
		if (tmp_cmd == NULL)
			return (NULL);
		*clist = create_command_list(clist, tmp_cmd);
	}
	return (*clist);
}
