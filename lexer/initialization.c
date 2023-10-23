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

# include "../minishell.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n > 0)
	{
		*p = c;
		p++;
		n--;
	}
	return (s);
}

/* signal handler function that is designed to take in SIGINT signal, which is generated
when the user presses Ctrl+C in the terminal; If the signal is received the handler prints
a new line, which effectively means that it moves to the next line */
void	handle_signals(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		//rl_replace_line("", 0); //!not working on mac!!!
		rl_redisplay();
	}
}

/*
Signals that need to be implemented similar to their bash behavior:
Ctrl+C -> move to a new line in the terminal (sends SIGINT signal),
Ctrl+D -> exit the shell,
Ctrl+\ -> terminate process and generate a core dump (sends SIGQUIT signal),
In interactive mode:
◦ ctrl-C displays a new prompt on a new line. 
◦ ctrl-D exits the shell.
◦ ctrl-\ does nothing.

SA_RESTART tells the operating system to restart system calls that were interrupted by a signal
rather than returning an error;
whenever a SIGINT signal is received, the handle function is called;
the last line sets the behavior of the SIGQUIT signal to be ignored;
*/
void	init_signals(t_minishell	*m, void (*handle)(int))
{
	m->sa.sa_handler = handle;
	m->sa.sa_flags = SA_RESTART;
	//sigaction(SIGINT, &m->sa, NULL);
	//signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/* first initialize all fields in the main data structure t_minishell to zero by using memset;
create libaries for the envp fields, one containing the full information and one only the 
parameter name, which is later being used as a search keyword */
void	init_minishell_struct_and_signals(t_minishell *m, char **envp)
{
	ft_memset(m, 0, sizeof(t_minishell));
	m->envp = create_envp_list(envp);
	init_signals(m, handle_signals);
}