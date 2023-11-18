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
// void free_on_exit(t_minishell *m, t_command *cmd)
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
	//free_to_null(cmd->path);
	//(void)cmd;
    //free_pipes(m);

//
	// if (cmd->in_file != NULL)
	// 	ft_file_name_clear(cmd->in_file);
	// free_all_filenames(cmd);
	// free_filename(cmd->in_redirects.file_name);
    // free_filename(cmd->out_redirects.file_name);
    // free_filename(cmd->in_redirects.new_heredoc_file);
    // free_filename(cmd->in_redirects.stop_heredoc);
}

// void ft_free_heredoc_on_exit(t_minishell *m, t_command *cmd)
// {
// 	free(tmp_line);

//     free_arr_to_null(m->path_buf);
//     free_to_null(m->line);
//     if (m->tlist)
//         ft_lstclear(&m->tlist, delete_token);
//     if (m->clist)
//         ft_lstclear(&m->clist, delete_cmd);
//     if (m->envp)
//         ft_lstclear(&m->envp, delete_envp);
//     free_intp_to_null(m->child_id);
//     free_pipes(m);

//     close(fd);
//                         // error_handling_and_exit("Error opening heredocs");
//     g_exit_code = EXIT_SUCCESS;
//     exit(errno);
// }

// void ft_free_in_redirections_per_cmd(t_minishell *m, t_command *cmd)
// {
// 	free_arr_to_null(m->path_buf);
//                 free_to_null(cmd->path);
//                 free_to_null(m->line);
// 	            if (m->tlist)
// 		            ft_lstclear(&m->tlist, delete_token);
// 	            if (m->clist)
// 		            ft_lstclear(&m->clist, delete_cmd);
// 	            if (m->envp)
// 		            ft_lstclear(&m->envp, delete_envp);
//                 free_pipes(m);
//                 free_intp_to_null(m->child_id);
//                 g_exit_code = EXIT_FAILURE;
//                 exit(g_exit_code);

// }
// void ft_free_no_cmd_no_exit(t_minishell *m, t_command *cmd)
// {
// 	free_filename(cmd->in_redirects.file_name);
//         free_filename(cmd->out_redirects.file_name);
//         free_filename(cmd->in_redirects.new_heredoc_file);
//         free_filename(cmd->in_redirects.stop_heredoc);
//         if (m->path_buf)
//             free_env(m->path_buf);
//         free_to_null(cmd->path);
//         free_to_null(m->line);
// 	    if (m->tlist)
// 	    	ft_lstclear(&m->tlist, delete_token);
// 	    if (m->clist)
// 		    ft_lstclear(&m->clist, delete_cmd);
// 	    if (m->envp)
// 		    ft_lstclear(&m->envp, delete_envp);
//         free(m->child_id);

// }


// void ft_free_no_cmd(t_minishell *m, t_command *cmd)
// {
// 	free_all_filenames(cmd);
//         free_arr_to_null(m->path_buf); 
//         free_to_null(cmd->path);
//         free_to_null(m->line);
// 	    if (m->tlist)
// 	    	ft_lstclear(&m->tlist, delete_token);
// 	    if (m->clist)
// 		    ft_lstclear(&m->clist, delete_cmd);
// 	    if (m->envp)
// 		    ft_lstclear(&m->envp, delete_envp);
//         free_pipes(m);
//         free_intp_to_null(m->child_id);
//         error_handling_and_exit("No valid command to run\n");

// }

// void ft_free_execuve(t_minishell *m, t_command *cmd)
// {
// 	if (cmd->in_file)
// 		    ft_file_name_clear(cmd->in_file);
//         free_to_null(cmd->path);
//         free_arr_to_null(arg_vec);
//         free_all(*m);
//         free_arr_to_null(m->path_buf);
//         free_intp_to_null(m->child_id);
//         free_all_filenames(cmd);
//         free_pipes(m);
//         error_handling_and_exit("Could not execute\n");

// }

// void free_execute_builtins(t_minishell *m, t_command *cmd)
// {
// 	if (m->child_id)
// 		free(m->child_id);
// 	if (m->path_buf)
// 		free_env(m->path_buf);
//     if (cmd->in_file != NULL)
// 		ft_file_name_clear(cmd->in_file);
// 	// if (cmd->in_file)
// 	// 	ft_lstclear(&cmd->in_file, free);

// 	// if (cmd->args)
//     // free_args(cmd->args);
	
// 	free_pipes(m);
// 	free_all(*m);

// }

