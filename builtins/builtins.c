/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/11/22 01:33:17 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_single_builtin_logic(t_minishell *m, t_command *cmd)
{
    int original_stdout = dup(STDOUT_FILENO);
    int original_stdin = dup(STDIN_FILENO);


	if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
        in_redirections_per_cmd_single_builtins(m, cmd);
	if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
    	output_redirect(m, cmd);
    m->status_code2 = execute_single_builtins(m, cmd);
	if (dup2(original_stdout, STDOUT_FILENO) == -1 || dup2(original_stdin, STDIN_FILENO) == -1)
    {
        perror("Failed to restore stdin or stdout");
        exit (42);
    }
    close(original_stdout);
	close(original_stdin);
	return (m->status_code2);
}


int	execute_single_builtins(t_minishell *m, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		m->status_code2 = echo(*m, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		m->status_code2 = pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		m->status_code2 = unset(m, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		m->status_code2 = env(m);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		m->status_code2 = export(m, cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		m->status_code2 = cd(m, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		m->status_code2 = exit_builtin(m, cmd);
	return (m->status_code2);
}

int	execute_builtins(t_minishell *m, t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		echo(*m, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		m->status_code2 = pwd();
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		m->status_code2 = unset(m, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		m->status_code2 = env(m);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		m->status_code2 = export(m, cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		m->status_code2 = cd(m, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		m->status_code2 = exit_builtin(m, cmd);
	free_m(m);
	free_pipes(m);
	exit (m->status_code2);
}
