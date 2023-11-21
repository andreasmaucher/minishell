/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:49:49 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/21 23:50:08 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void in_redirections_per_single_bultins_cmd_file_read(t_minishell *m, t_command *cmd, t_list *tmp)
{
    if (check_file_rights((char *)tmp->value) == 0)
        {
            cmd->in_redirects.fd_write = open((char *)tmp->value, O_RDONLY, 0777);
            if (cmd->in_redirects.fd_write == -1)
                perror("Cant open the file\n");
            if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
                perror("Input IN-redirection isn't working\n");
            close(cmd->in_redirects.fd_write);
        }
        else
        {
            perror("Cant read from input file, permission denied\n");
            if (tmp->value)
                {
                    free_m(m);
                    free_pipes(m);
                    m->status_code2 = errno;
                }
        }
}

int in_redirections_per_cmd_single_builtins(t_minishell *m, t_command *cmd)
{
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp->next != NULL)
    {
        if (check_file_rights((char *)tmp->value) == 0)
        {
            //free_out_file_list(cmd->in_file); //!!! incorrect free, insert correct one
        }
        else
        {
            perror("Cant read from input file, permission denied\n");
            if (tmp->value)
            {
                free_m(m);
                free_pipes(m);
                m->status_code2 = errno;               
            }
        }
        tmp = tmp->next;
    }
    in_redirections_per_single_bultins_cmd_file_read(m, cmd, tmp);
    return (0);
}

void in_redirections_per_cmd_file_read(t_minishell *m, t_command *cmd, t_list *tmp)
{
    if (check_file_rights((char *)tmp->value) == 0)
    {
        cmd->in_redirects.fd_write = open((char *)tmp->value, O_RDONLY, 0777);
        if (cmd->in_redirects.fd_write == -1)
            perror("Cant open the file\n");
        if (dup2(cmd->in_redirects.fd_write, STDIN_FILENO) == -1)
            perror("Input IN-redirection isn't working\n");
        close(cmd->in_redirects.fd_write);
    }
    else
    {
        perror("Cant read from input file, permission denied\n");
        if (tmp->value)
            {
                free_m(m);
                free_pipes(m);
                error_handling_and_exit("Cant read from input file, permission denied");
            }
    }
}

int in_redirections_per_cmd(t_minishell *m, t_command *cmd)
{
    t_list *tmp;

    tmp = cmd->in_file;
    while (tmp->next != NULL)
    {
        if (check_file_rights((char *)tmp->value) == 0)
        {
            //free_out_file_list(cmd->in_file); //!!! incorrect free, insert correct one
        }
        else
        {
            perror("Cant read from input file, permission denied\n");
            if (tmp->value)
            {
                free_m(m);
                free_pipes(m);
                m->status_code = errno;
                error_handling_and_exit("Cant read from input file, permission denied");
            }
        }
        tmp = tmp->next;
    }
    in_redirections_per_cmd_file_read(m, cmd, tmp);
    return (0);
}
