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

/* delete a node in the env list */

void free_m(t_minishell *m)
{
	free_arr_to_null(m->path_buf);
    free_to_null(m->line);
	free_intp_to_null(m->child_id);
	if (m->tlist)
        ft_lstclear(&m->tlist, delete_token);
    if (m->clist)
        ft_lstclear(&m->clist, delete_cmd);
    if (m->envp)
		ft_lstclear(&m->envp, delete_envp);
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
