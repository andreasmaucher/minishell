/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/11/22 01:50:23 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal_switch;

/*
TIOCSTI: insert the given byte into the input queue
*/
void	handle_sigint_child(int signum)
{
	if (signum == SIGINT)
	{
		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
		g_signal_switch = 1;
	}
}

void	handle_sigint_block_cmd(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		signal(SIGINT, SIG_IGN);
	}
}

void	handle_sigint_switch(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_switch = 1;
	}
}

/* signal handler function that is designed to take in SIGINT signal, 
which is generatedwhen the user presses Ctrl+C in the terminal;
SIGINT (Ctrl+C) with exit code 130; */
void	handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_switch = 2;
	}
}

/* Signals that need to be implemented similar to their bash behavior:
Ctrl+C -> move to a new line in the terminal (sends SIGINT signal),
Ctrl+D -> exit the shell,
Ctrl+\ -> terminate process and generate a core dump (sends SIGQUIT signal),
In interactive mode:
◦ ctrl-C displays a new prompt on a new line. 
◦ ctrl-D exits the shell.
◦ ctrl-\ does nothing.

SA_RESTART tells the operating system to restart system calls that were 
interrupted by a signal, rather than returning an error;
whenever a SIGINT signal is received, the handle function is called;
the last line sets the behavior of the SIGQUIT signal to be ignored; */
void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
