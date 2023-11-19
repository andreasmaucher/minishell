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
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		echo(*m, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		m->status_code2 = pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		m->status_code = unset(m, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		m->status_code = env(m);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		m->status_code = export(m, cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		m->status_code = cd(m, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		m->status_code = exit_builtin(m, cmd);
	return (m->status_code);
}

int	execute_builtins(t_minishell *m, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		echo(*m, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		m->status_code = pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		m->status_code = unset(m, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		m->status_code = env(m);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		m->status_code = export(m, cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		m->status_code = cd(m, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		m->status_code = exit_builtin(m, cmd);
	free_m(m);
	free_pipes(m);
	exit (m->status_code);
}
