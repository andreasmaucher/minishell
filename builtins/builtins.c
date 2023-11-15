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

int	execute_single_builtins(t_minishell *m, t_command *cmd)
{
	int	return_code;

	return_code = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return_code = echo(*m, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return_code = pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return_code = unset(m, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return_code = env(m);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return_code = export(m, cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return_code = cd(m, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return_code = exit_builtin(m, cmd);
	return (return_code);
}

int	execute_builtins(t_minishell *m, t_command *cmd)
{
	int	return_code;

	return_code = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return_code = echo(*m, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return_code = pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return_code = unset(m, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return_code = env(m);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return_code = export(m, cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return_code = cd(m, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return_code = exit_builtin(m, cmd);

	//this was added to free
	//free_all(*m);
	// if (m->line)
	// 	m->line = set_pt_to_null(m->line);
	// if (m->tlist)
	// 	ft_lstclear(&m->tlist, delete_token);
	// if (m->clist)
	// 	ft_lstclear(&m->clist, delete_cmd);
	// if (m->envp)
	// 	ft_lstclear(&m->envp, delete_envp);
	if (m->child_id)
		free(m->child_id);
	if (m->path_buf)
		free_env(m->path_buf);
    if (cmd->in_file != NULL)
		ft_file_name_clear(cmd->in_file);
	// if (cmd->in_file)
	// 	ft_lstclear(&cmd->in_file, free);

	// if (cmd->args)
    // free_args(cmd->args);
	
	free_pipes(m);
	free_all(*m);

	// if (cmd->args)
	// {
	// free_args(cmd->args);

	// // if (cmd)
	// // 	free(cmd);
	// cmd = set_pt_to_null(cmd);
	// }

	exit (return_code);
}