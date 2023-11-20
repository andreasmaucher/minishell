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

int		g_exit_code;
int		g_signal;

/* first initialize all fields in the main data structure t_minishell 
to zero by using memset;create libaries for the envp fields, one containing
the full information and one only the parameter name, which is later being
used as a search keyword */
t_command	*init_minishell_struct_and_signals(t_minishell *m, char **envp)
{
	t_command	*cmd;

	cmd = NULL;
	init_signals();
	ft_memset(m, 0, sizeof(t_minishell));
	m->status_code2 = 0;
	m->envp = create_envp_list(envp);
	return (cmd);
}

int restore_stdin_stdout_main(void)
{
	int default_stdin;
    int default_stdout;
	
	default_stdin = dup(STDIN_FILENO);
    default_stdout = dup(STDOUT_FILENO);
	if (dup2(default_stdin, 0) == -1 || dup2(default_stdout, 1) == -1)
    {
        perror("Failed to restore stdin or stdout");
        exit (42);
    }
	close(default_stdin);
	close(default_stdout);
	return (0);
}

void free_lists(t_minishell *m)
{
	if (m->tlist)
		ft_lstclear(&m->tlist, delete_token);
	if (m->clist)
		ft_lstclear(&m->clist, delete_cmd);
}

void free_memory_for_next_line(t_minishell *m)
{
	if (m->line)
		m->line = set_pt_to_null(m->line);
	free_lists(m);
	term_processes(m);
}

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
	t_command *cmd;

	(void)av;
	if (ac != 1)
		return (1);
	cmd = init_minishell_struct_and_signals(&m, envp);
	while(1)
	{
		restore_stdin_stdout_main();
		m.line = readline("Myshell: ");
		if (!m.line)
			exit_shell(m);
		else if (*m.line == '\0')
			continue ;
		add_history(m.line);
		m.tlist = split_line_into_tokens(m);
		m.clist = parser(m);
		m.status_code2 = executor(m, cmd, envp);
		free_memory_for_next_line(&m);
	}
}