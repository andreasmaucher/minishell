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

/* used for exiting the program and freeing the memory correctly */
void	*set_pt_to_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

/* helper function for deleting a token and freeing its memory */
void	delete_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (content == NULL)
	{
		return ;
	}
	if (token->str != NULL)
	{
		free(token->str);
	}
	free(token);
}

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

/*
deletes a command node and all args in it;
*/
void	delete_cmd(void *arg)
{
	t_command	*command;

	command = (t_command *)arg;
	if (command->args)
		free_args(command->args);
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


	//? more stuff
	exit(g_exit_code);
}
