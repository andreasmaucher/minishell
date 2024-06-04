/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:49:49 by mrizakov          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	in_red_single_bultin_file(t_minishell *m, t_command *cmd, t_list *tmp)
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

int	in_red_per_cmd_single_builtins(t_minishell *m, t_command *cmd)
{
	t_list	*tmp;

	tmp = cmd->in_file;
	while (tmp->next != NULL)
	{
		if (check_file_rights((char *)tmp->value) == 0)
		{
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
	in_red_single_bultin_file(m, cmd, tmp);
	return (0);
}

void	in_red_per_cmd_file_read(t_minishell *m, t_command *cmd, t_list *tmp)
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
			error_handling_and_exit("Permission denied");
		}
	}
}

int	in_redirections_per_cmd(t_minishell *m, t_command *cmd)
{
	t_list	*tmp;

	tmp = cmd->in_file;
	while (tmp->next != NULL)
	{
		if (check_file_rights((char *)tmp->value) == 0)
		{
		}
		else
		{
			perror("Cant read from input file, permission denied\n");
			if (tmp->value)
			{
				free_m(m);
				free_pipes(m);
				m->status_code = errno;
				error_handling_and_exit("Permission denied");
			}
		}
		tmp = tmp->next;
	}
	in_red_per_cmd_file_read(m, cmd, tmp);
	return (0);
}
