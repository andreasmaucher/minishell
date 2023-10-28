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
	exit (return_code);
}
