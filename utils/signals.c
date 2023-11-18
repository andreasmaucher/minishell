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

/*
if the minishell is called inside minishell signals to the parent need to
be ignored
*/
/* void	handle_sigint_within_heredoc(int signum)
{
	{
		signal(SIGINT, SIG_IGN);
		raise(SIGQUIT);
		if (signum == SIGINT)
			g_exit_code = 130;
		exit(1);
	}
} */

void		ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		ft_putchar_fd(s[i], fd);
		i++;
	}
}

// SIGINT handler for parent processs
static void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_redisplay();
	}
}

// Register SIGINT handler and ignore SIGQUIT for parent process
void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

// SIGINT and SIGQUIT handler for child process
static void	child_signal_handler(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	else if (signal == SIGINT)
		ft_putchar_fd('\n', STDERR_FILENO);
}

// Register SIGINT and SIGQUIT handler for child process
void	handle_child_signals(void)
{
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, child_signal_handler);
}

/* 
void	handle_sigint_child(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_code = 130;
		exit (130);
	}
}


after the execution in the child is finished the parent needs to handle
the signals again

void	handle_sigint_parent(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		g_exit_code = 130;
	}
}

signal handler function that is designed to take in SIGINT signal, 
which is generatedwhen the user presses Ctrl+C in the terminal;
SIGINT (Ctrl+C) with exit code 130;

void	handle_sigint(int signal)
{
	//if (g_signal == 0)
		if (signal == SIGINT)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			g_exit_code = 130;
		}
}


Signals that need to be implemented similar to their bash behavior:
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
the last line sets the behavior of the SIGQUIT signal to be ignored;
 void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
} */