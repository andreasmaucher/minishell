/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_processes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:55:36 by mrizakov          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_codes(t_minishell *m, int wstatus)
{
	if (g_signal_switch == 0 && wstatus == 0)
		m->status_code2 = 0;
	else if (g_signal_switch == 0 && wstatus == 512)
		m->status_code2 = 127;
	else if (g_signal_switch == 0 && wstatus == 3328)
		m->status_code2 = 126;
	else if (g_signal_switch != 0 && wstatus == 0)
		m->status_code2 = 130;
	else if (g_signal_switch == 0 && wstatus == 256)
		m->status_code2 = 1;
	else if (g_signal_switch == 0 && wstatus != 0)
		m->status_code2 = 1;
}

int	wait_processes(t_minishell *m)
{
	int		i;
	int		wstatus;
	pid_t	pid;

	wstatus = 0;
	i = 0;
	pid = 0;
	while (i <= m->pipe_n && m->forked == 1)
	{
		pid = waitpid(m->child_id[i], &wstatus, 0);
		if (pid == m->child_id[m->pipe_n])
		{
			exit_codes(m, wstatus);
			signal(SIGINT, handle_sigint);
			return (m->status_code2);
		}
		i++;
	}
	g_signal_switch = 0;
	return (m->status_code2);
}

void	kill_process(t_minishell *m, int process_id)
{
	int	i;

	i = 0;
	while (i <= process_id)
	{
		kill(m->child_id[i], SIGTERM);
		i++;
	}
}

void	term_processes(t_minishell *m)
{
	int	j;

	j = 0;
	if (m->forked == 1)
	{
		while (j <= m->pipe_n)
		{
			kill(m->child_id[j], SIGTERM);
			j++;
		}
		m->forked = 0;
	}
}
