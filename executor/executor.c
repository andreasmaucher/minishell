/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:56:22 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 00:24:41 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int single_cmd(t_minishell *m, t_command *cmd, char **envp)
{   
    m->forked = 1;
    m->child_id[0] = fork();
    if (m->child_id[0] == -1) 
        error_handling_and_exit("Fork failed\n");
    if (m->child_id[0] == 0)
    {
        cmd->path = NULL;
        if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
            in_redirections_per_cmd(m, cmd);
        if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
            if (output_redirect(m, cmd) != 0)
                cmd->args = NULL; //output_redirect(m, cmd);
        if (cmd->args)
            cmd->path = valid_path(m->path_buf, cmd->args[0]);
        if (cmd->path == NULL)
            no_cmd(cmd, m);
        if (cmd->type != BUILTIN)
            execute_program(cmd->args, cmd, m, envp);
    }
    free_all_filenames(cmd);
    return (0);
}

void multiple_cmd_piping(t_minishell *m, t_command *cmd)
{
    cmd->path = NULL;
    if (m->current_process_id == 0 )
        dup2(m->pipes[m->current_process_id][1], STDOUT_FILENO);
    else if (m->current_process_id == m->pipe_n )
        dup2(m->pipes[m->current_process_id - 1][0], STDIN_FILENO);
    else
    {
        dup2(m->pipes[m->current_process_id - 1][0], STDIN_FILENO);
        dup2(m->pipes[m->current_process_id][1], STDOUT_FILENO);
    }
    if (cmd->input_redir_type == REDIRECT_IN || cmd->input_redir_type == REDIRECT_HEREDOC)
        in_redirections_per_cmd(m, cmd);
    if (cmd->output_redir_type == REDIRECT_OUT || cmd->output_redir_type == REDIRECT_APPEND)
            if (output_redirect(m, cmd) != 0)
                cmd->args = NULL;
    close_pipes(m);
}

int multiple_cmd(t_minishell *m, t_command *cmd, char **envp)
{
    m->forked = 1;
    m->child_id[m->current_process_id] = fork();
    if (m->child_id[m->current_process_id] == -1)
        error_handling_and_exit("Fork failed\n");
    if (m->child_id[m->current_process_id] == 0)
    {
        multiple_cmd_piping(m, cmd);
        if (cmd->args)
            cmd->path = valid_path(m->path_buf, cmd->args[0]);
        if (cmd->path == NULL)
            no_cmd(cmd, m);
        if (cmd->type == BUILTIN)
        {
            free_to_null(cmd->path);
            free_all_filenames(cmd);
            execute_builtins(m, cmd);
        }
        if (cmd->type != BUILTIN)
            execute_program(cmd->args, cmd, m, envp);
    }
    free_all_filenames(cmd);
    m->current_process_id++;
    return (0);
}

int	execute_program(char **arg_vec, t_command *cmd, t_minishell *m, char **envp)
{
    int i;

    i = 0;
    while (arg_vec[i])
        i++;
    if (execve(cmd->path, arg_vec, envp) == -1)
    {
        free_m(m);
        if (cmd->in_file)
		    ft_file_name_clear(cmd->in_file);
        free_to_null(cmd->path);
        free_arr_to_null(arg_vec);
        free_all_filenames(cmd);
        free_pipes(m);
        error_handling_and_exit("Could not execute\n");
    }
    return (0);
}

int executor(t_minishell m, t_command *cmd, char **envp)
{
    t_list *tmp;

    init_executor(&m);
    tmp = m.clist;
    while(tmp)
    {
        cmd = (t_command *) tmp->value;
        if (cmd->input_redir_type == REDIRECT_HEREDOC || cmd->input_redir_type == REDIRECT_IN)
            ft_heredoc(&m, cmd);
        if (cmd->type == BUILTIN && m.pipe_n == 0)
            m.status_code2 = execute_single_builtin_logic(&m, cmd);
        if (m.pipe_n == 0 && cmd->type != BUILTIN)
            single_cmd(&m, cmd, envp);
        if (m.pipe_n > 0)
            multiple_cmd(&m, cmd, envp);
        tmp = tmp->next;
    }
    m.status_code2 = exit_executor(&m);
    return (m.status_code2);
}