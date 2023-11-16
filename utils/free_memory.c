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
free args element of command list
*/
void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		i++;
	}
	free(args);
}

void	delete_file(void *content)
{
	t_file	*file;

	file = (t_file *)content;
	if (content == NULL)
		return ;
	if (file->file_name != NULL)
		free_to_null(file->file_name);
	free (file);
}

void	free_out_file_list(t_list *out_file)
{
	t_list		*current;
	t_list		*next;

	current = out_file;
	while (current != NULL) 
	{
		free(current->value);
		current->value = NULL;
		next = current->next;
		free(current);
		current = next;
	}
}

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
	exit(g_exit_code);
}
