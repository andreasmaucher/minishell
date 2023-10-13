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

# include "../minishell.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == 0)
		return (0);
	while (lst->next != 0)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*prev;

	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
	{
		prev = ft_lstlast(*lst);
		prev->next = new;
	}
}

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (node == 0)
		return (0);
	node->value = content;
	node->next = 0;
	return (node);
}

/* adds a new node to a list; in case the list is empty, the new node becomes the head, else 
it is added at the end of the list */
t_list *create_command_list(t_list **clist, t_command *tmp_cmd)
{
	t_list *new_node;
	
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
	cmd->arguments = NULL;
	cmd->type = PATH;
	cmd->before_pipe = false;
	cmd->after_pipe = false;
	cmd->redir_type = 0;
	cmd->redir_type = 0;
	cmd->inred_file = NULL;
	cmd->outred_file = NULL;
	return (cmd);
}

/* counts the amount of commands (len) in a token list, determined by the amount of PIPES;
iteration by 1 at the end to account for the fact, that a PIPE is always splitting two parts */
int command_count(t_list *tlist) 
{
    t_list *tmp_head;
    t_token *token;
    int len;
    
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
    return(len);
}

//! CHECK
/* initialization of clist to NULL, to ensure that it starts as an empty list */
t_list *setup_command_list(t_list **clist, t_list *tlist)
{
    int len;
    t_command   *tmp_cmd;

    *clist = NULL;
    len = command_count(tlist);
    while (len--)
    {
        tmp_cmd = ft_create_cmd();
        if (tmp_cmd == NULL)
            return (NULL);
        *clist = create_command_list(clist, tmp_cmd);
    }
    return(*clist);
}