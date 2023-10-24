
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
    while (1)
	{
        if (dup2(m.stdin_original, 0) == -1)
        {
            perror("Failed to restore stdin");
            return (1);
        }
        m.line = readline("Myshell: ");
		add_history(m.line);
//		if (ft_strcmp(m.line, "exit") == 0)
//            m.line = readline("Myshell: ");
        if (m.line != NULL)
            m.tlist = split_line_into_tokens(m, envp);
        if (m.tlist != NULL)
        {
            printlist(m.tlist); //! only for testing
            m.clist = parser(m);
        }
        if (m.clist != NULL)
		    executor(m, envp);
		ft_lstclear(&m.tlist, token_del);
		ft_lstclear(&m.clist, command_del);
        free(m.line);
	}
}
