/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_processes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:55:36 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/21 23:56:27 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int wait_processes(t_minishell *m)
{
    int i;
    int wstatus;
    pid_t pid;
    
    wstatus = 0;
    i = 0;
    pid = 0;
    while (i <= m->pipe_n && m->forked == 1)
    {
        pid = wait(&wstatus);
        if (pid == m->child_id[m->pipe_n])
        {
            m->status_code2 = wstatus;
            if (WIFEXITED(wstatus)) 
                m->status_code2 = WEXITSTATUS(wstatus);
            if (WIFSIGNALED(wstatus)) 
            {
                m->status_code2 = WTERMSIG(wstatus);
                m->status_code2 = WIFSIGNALED(wstatus);
            }
        }
        m->status_code2 = m->status_code2 % 255;
        i++;
    }
    return(m->status_code2);
}

void kill_process(t_minishell *m, int process_id)
{
    int i;
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