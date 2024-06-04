/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:54:32 by mrizakov          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_executor(t_minishell *m)
{
	m->current_process_id = 0;
	m->pipe_n = command_count(m->tlist) - 1;
	m->child_id = malloc(sizeof(int) * (m->pipe_n +1));
	m->path_buf = find_path_executor(m->envp);
	if (m->pipe_n > 0)
	{
		initialize_pipes(m);
		m->forked = 1;
	}
	return (0);
}

int	initialize_pipes(t_minishell *m)
{
	int	i;

	i = 0;
	m->pipes = malloc(sizeof (int *) * m->pipe_n * 2);
	while (i <= m->pipe_n)
	{
		m->pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(m->pipes[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}

int	free_pipes(t_minishell *m)
{
	int	i;

	i = 0;
	if (m->pipe_n == 0)
		return (0);
	while (i <= m->pipe_n)
	{
		if (m->pipes[i])
			free(m->pipes[i]);
		i++;
	}
	if (m->pipes)
		free(m->pipes);
	return (0);
}

int	close_pipes(t_minishell *m)
{
	int	i;

	i = 0;
	while (i <= m->pipe_n)
	{
		close(m->pipes[i][0]);
		close(m->pipes[i][1]);
		i++;
	}
	return (0);
}

int	exit_executor(t_minishell *m)
{
	if (m->pipe_n > 0)
	{
		close_pipes(m);
		free_pipes(m);
	}
	m->status_code2 = wait_processes(m);
	free_arr_to_null(m->path_buf);
	free_intp_to_null(m->child_id);
	m->forked = 0;
	return (m->status_code2);
}
