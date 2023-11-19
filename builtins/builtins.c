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


int	execute_single_builtin_logic(t_minishell *m, t_command *cmd)
{
	// int original_stdout;
	
	// original_stdout = dup(STDOUT_FILENO);
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
	// dup2(original_stdout, STDOUT_FILENO); //
	// dup2(original_stdin, STDIN_FILENO); //

    close(original_stdout);
	close(original_stdin);



	// dup2(original_stdout, STDOUT_FILENO);
    // close(original_stdout);
	return (m->status_code2);
}


int	execute_single_builtins(t_minishell *m, t_command *cmd)
{
	// int original_stdout;
	
	// original_stdout = dup(STDOUT_FILENO);
	printf("in Bulitins m->status_code is = %d\n", m->status_code2);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		m->status_code2 = echo(*m, cmd);
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
	// dup2(original_stdout, STDOUT_FILENO);
    // close(original_stdout);
	return (m->status_code);
}

int	execute_builtins(t_minishell *m, t_command *cmd)
{
	// int	return_code;

	// return_code = 0;
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
	// if (m->child_id)
	// 	free(m->child_id);
	// if (m->path_buf)
	// 	free_env(m->path_buf);
	free_m(m);


    // if (cmd->in_file != NULL)
	// 	ft_file_name_clear(cmd->in_file);


	// if (cmd->in_file)
	// 	ft_lstclear(&cmd->in_file, free);

	// if (cmd->args)
    // free_args(cmd->args);
	
	free_pipes(m);
	// free_all(*m);

	// if (cmd->args)
	// {
	// free_args(cmd->args);

	// // if (cmd)
	// // 	free(cmd);
	// cmd = set_pt_to_null(cmd);
	// }
	exit (m->status_code);
}