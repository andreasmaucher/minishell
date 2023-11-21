/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:48:48 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/21 23:49:25 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int output_redirect_file_write(t_list *tmp, t_command *cmd)
{
    if (tmp->is_append == 0)
    {
        cmd->out_redirects.fd_write = open(tmp->value, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (cmd->out_redirects.fd_write == -1)
        {
            free(cmd->args);
            perror("Could not open output file\n");
        }
        if (dup2(cmd->out_redirects.fd_write, STDOUT_FILENO) == -1)
            return (1);
        close(cmd->out_redirects.fd_write);
    }
    if (tmp->is_append == 1)
    {
        cmd->out_redirects.fd_write = open(tmp->value, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
        if (cmd->out_redirects.fd_write == -1)
        {
            free(cmd->args);
            perror("Could not open output file\n");
        }
        if (dup2(cmd->out_redirects.fd_write, STDOUT_FILENO) == -1)
            return (1);
        close(cmd->out_redirects.fd_write);
    }
    return (0);
}

int output_redirect(t_minishell *m, t_command *cmd)
{
    t_list *tmp;
    int     fd;

    tmp = cmd->out_file;

    while (tmp != NULL)
    {
        if (tmp->next == NULL)
        {
            if (if_file_exists_delete(tmp->value) != 0 && output_redirect_file_write(tmp, cmd) != 0)
                return (1);
        }
        else
        {
            if_file_exists_delete(tmp->value);
            fd = open(tmp->value, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); // S_IRUSR | S_IWUSR
            if (fd == -1) 
            {
                free_m(m);
                free_pipes(m);
                return (1);
            }
            close(fd);
        }
        tmp = tmp->next;
    }
    free_filename(cmd->out_redirects.file_name);
    return(0);
}

int free_in_redirects_file(t_minishell *m)
{
    t_command *cmd_free;
    t_list *tmp;
    tmp = m->clist;
    cmd_free= NULL;
    while(tmp)//(m->clist)
    {
        cmd_free = (t_command *) tmp->value;//m->cl
        if (cmd_free->in_redirects.file_name)
            free(cmd_free->in_redirects.file_name);
        tmp = tmp->next;//m->clist = m->clist->next;
    }
return (0);
}
