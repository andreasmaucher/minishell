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

void	free_to_null(char *var)
{
	if (var || var != NULL)
		free(var);
	var = NULL;
}

void	free_intp_to_null(int *var)
{
	if (var || var != NULL)
		free(var);
	var = NULL;
}

void	free_all_the_og(t_minishell m)
{
	if (m.line != NULL)
		m.line = set_pt_to_null(m.line);
	if (m.path_buf != NULL)
		free_arr_to_null(m.path_buf);
	if (m.child_id != NULL)
	{
		free(m.child_id);
		m.child_id = NULL;
	}
	if (m.tlist)
		ft_lstclear(&m.tlist, delete_token);
	if (m.clist)
		ft_lstclear(&m.clist, delete_cmd);
	if (m.envp)
		ft_lstclear(&m.envp, delete_envp);
	free_pipes(&m);
}

void	free_cmd_the_og(t_command *cmd)
{
	if (cmd->args != NULL)
		free_arr_to_null(cmd->args);
}