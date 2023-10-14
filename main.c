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

/* shell is only created if there is exactly one argument (name of the executable);
m.line == NULL to exit if the user calls Ctrl+D or simply if "exit" is called;
tlist = tokenlist, meaning the list that holds all tokens,
clist = commandlist, meaning the list that holds all commands */
int main(int ac, char **av, char **envp)
{
	t_minishell m;

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
		if (m.line == NULL || ft_strcmp(m.line, "exit") == 0) 
			exit_shell(m);
		m.tlist = split_line_into_tokens(m, envp);
		printlist(m.tlist); //! only for testing
		m.clist = parser(m);
		ft_lstclear(&m.tlist, token_del);
		ft_lstclear(&m.clist, command_del);
		//! if execve -1 free **args of command_list
		free(m.line);
	}
}
