/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:48:48 by mrizakov          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	write_fail(t_command *cmd)
{
	if (cmd->out_redirects.fd_write == -1)
	{
		free(cmd->args);
		perror("Could not open output file\n");
	}
	if (dup2(cmd->out_redirects.fd_write, STDOUT_FILENO) == -1)
		return (1);
	close(cmd->out_redirects.fd_write);
	return (0);
}

int	output_redirect_file_write(t_list *tmp, t_command *cmd)
{
	if (tmp->is_append == 0)
	{
		cmd->out_redirects.fd_write = open(tmp->value, 
				O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if (write_fail(cmd) == 1)
			return (1);
	}
	if (tmp->is_append == 1)
	{
		cmd->out_redirects.fd_write = open(tmp->value, 
				O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
		if (write_fail(cmd) == 1)
			return (1);
	}
	return (0);
}

int	output_redirect_delete_open(t_minishell *m, t_list *tmp)
{
	int	fd;

	if_file_exists_delete(tmp->value);
	fd = open(tmp->value, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) 
	{
		free_m(m);
		free_pipes(m);
		return (1);
	}
	close(fd);
	return (0);
}

int	output_redirect(t_minishell *m, t_command *cmd)
{
	t_list	*tmp;

	tmp = cmd->out_file;
	while (tmp != NULL)
	{
		if (tmp->next == NULL)
		{
			if (if_file_exists_delete(tmp->value) != 0 
				&& output_redirect_file_write(tmp, cmd) != 0)
				return (1);
		}
		else
		{
			if (output_redirect_delete_open(m, tmp) == 1)
				return (1);
		}
		tmp = tmp->next;
	}
	free_filename(cmd->out_redirects.file_name);
	return (0);
}

int	free_in_redirects_file(t_minishell *m)
{
	t_command	*cmd_free;
	t_list		*tmp;

	tmp = m->clist;
	cmd_free = NULL;
	while (tmp)
	{
		cmd_free = (t_command *) tmp->value;
		if (cmd_free->in_redirects.file_name)
			free(cmd_free->in_redirects.file_name);
		tmp = tmp->next;
	}
	return (0);
}
