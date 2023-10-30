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

#include "minishell.h"

int g_exit_code;

/* 
shell is only created if there is exactly one argument
(name of the executable);
m.line == NULL to exit if the user calls Ctrl+D or simply if "exit" is called;
tlist = tokenlist, meaning the list that holds all tokens,
clist = commandlist, meaning the list that holds all commands;
if (!m.line) -> this condition is to mimic the behavior of Ctrl+D and signals
that there's no more input and closes the shell
*/
int main(int ac, char **av, char **envp)
{
	t_minishell m;

	(void)av;
	if (ac != 1)
		return (1);
	init_minishell_struct_and_signals(&m, envp);
	int default_stdin; // Duplicate stdin (file descriptor 0)
    int default_stdout; // Duplicate stdout (file descriptor 1)
	while(1)
	{
		//!input not being restored after command "<out98 cat"
		default_stdin = dup(0); // Duplicate stdin (file descriptor 0)
    	default_stdout = dup(1); // Duplicate stdout (file descriptor 1)
		if (dup2(default_stdin, 0) == -1)
        {
            perror("Failed to restore stdin");
            return (1);
        }
		close(default_stdin);
		if (dup2(default_stdout, 1) == -1)
        {
            perror("Failed to restore stdout");
            return (1);
        }
		close(default_stdout);
		m.line = readline("Myshell: ");
		if (!m.line)
			exit_shell(m);
		//printf("Passed m.line\n");
		add_history(m.line);
		m.tlist = split_line_into_tokens(m);
		//printlist(m.tlist); //! only for testing
		m.clist = parser(m);
		executor(m, envp);
		if (m.line)
			m.line = set_pt_to_null(m.line);
		if (m.tlist)
			ft_lstclear(&m.tlist, delete_token);
		if (m.clist)
			ft_lstclear(&m.clist, delete_cmd);
		//! if execve -1 free **args of command_list
		// int i = 0;

		// while(m.child_id[i])
		// {
		// 	kill(m.child_id[i], SIGTERM);
		// 	i++;
		// }
		// kill(m.child_id[0], SIGTERM);
		//printf("End of main M.line is :|%s|\n", m.line);
		//printf("End of main M.line is :|%s|\n", m.line);
		term_processes(m); // is this needed?
	}
}
