/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_processes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:55:36 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 02:09:56 by mrizakov         ###   ########.fr       */
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
    // if (m->pipe_n == 0 && m->forked == 0 && g_signal_switch == 2)
    // {
    //     m->status_code2 = 130;
    //     g_signal_switch = 0;
    //     return(m->status_code2);
    // }
    while (i <= m->pipe_n && m->forked == 1)
    {
        pid = wait(&wstatus);
        printf("Errno for process n %d is  %d\n", i, wstatus);
        printf("g_signal_switch for process n %d is %d\n", i, g_signal_switch);
        if (pid == m->child_id[m->pipe_n])
        {
            printf("Errno for last procees is  %d\n", wstatus);
            printf("g_signal_switch for last process is %d\n", g_signal_switch);

            // m->status_code2 = wstatus;
            if (g_signal_switch != 0 || wstatus != 0)
            {
                m->status_code2 = 130;
                return(m->status_code2);

            }
            // if ((WIFSIGNALED(wstatus)))
            //     m->status_code2 = 55;

            //     // m->status_code2 = WTERMSIG(wstatus);
            // else if ((WIFEXITED(wstatus)))
            //     m->status_code2 = WEXITSTATUS(wstatus);
            // else if ((WIFSTOPPED(wstatus)))
            //     m->status_code2 = 1;    
            // if (g_signal_switch == 0)
            //     m->status_code2 = g_signal_switch;
        }
        // m->status_code2 = m->status_code2 % 255;
        i++;
    }
    g_signal_switch = 0;
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