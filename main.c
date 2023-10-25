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

int global_exit_code;

/* 
shell is only created if there is exactly one argument (name of the executable);
m.line == NULL to exit if the user calls Ctrl+D or simply if "exit" is called;
tlist = tokenlist, meaning the list that holds all tokens,
clist = commandlist, meaning the list that holds all commands;
if (!m.line) -> this condition is to mimic the behavior of Ctrl+D and signals
that there's no more input and closes the shell
*/
int main(int ac, char **av, char **envp)
{
	t_minishell m;
	t_command *cmd = NULL;

	(void)av;
	if (ac != 1)
		return (1);
	init_minishell_struct_and_signals(&m, envp);
	while(1)
	{
		m.line = readline("Myshell: ");
		if (!m.line)
			exit_shell(m);
		add_history(m.line);
		m.tlist = split_line_into_tokens(m);
		printlist(m.tlist); //! only for testing
		m.clist = parser(m);
		cmd = (t_command *) m.clist->value;
		execute_builtins(&m, cmd);
		//executor(m, envp);
		ft_lstclear(&m.tlist, token_del);
		ft_lstclear(&m.clist, command_del);
		//! if execve -1 free **args of command_list
		free(m.line);
	}
}
