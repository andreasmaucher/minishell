/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 04:21:23 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 04:22:30 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
deletes a command node and all args in it;
*/
void	delete_cmd(void *arg)
{
	t_command	*command;

	command = (t_command *)arg;
	if (command->args)
		free_args(command->args);
	if (command->out_file)
		free_out_file_list(command->out_file);
	if (command->in_file != NULL)
		ft_file_name_clear(command->in_file);
	command = set_pt_to_null(command);
}

/*
to free up all memory in the end, including memory automatically
allocated by readline function;
this function is either called by the exit command or by pressing
Ctrl+D
*/
int	exit_shell(t_minishell m)
{
	if (m.line)
		m.line = set_pt_to_null(m.line);
	if (m.tlist)
		ft_lstclear(&m.tlist, delete_token);
	if (m.clist)
		ft_lstclear(&m.clist, delete_cmd);
	if (m.envp)
		ft_lstclear(&m.envp, delete_envp);
	if (m.child_id)
		free(m.child_id);
	if (m.path_buf)
		free_env(m.path_buf);
	exit(m.status_code);
}

/*
deletes an entire linked list
*/
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!del)
		return ;
	while (lst && *lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
}
