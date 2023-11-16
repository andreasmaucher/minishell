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

int restore_stdin_stdout(void)
{
	int default_stdin; // Duplicate stdin (file descriptor 0)
    int default_stdout; // Duplicate stdout (file descriptor 1)
	
	default_stdin = dup(STDIN_FILENO); // Duplicate stdin (file descriptor 0)
    default_stdout = dup(STDOUT_FILENO); // Duplicate stdout (file descriptor 1)
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
	term_processes(m); // is this needed?
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
			if (restore_stdin_stdout() != 0)
			exit(42);
		m.line = readline("Myshell: ");
		if (!m.line)
			exit_shell(m);
		else if (*m.line == '\0')
			continue ;
		add_history(m.line);
		m.tlist = split_line_into_tokens(m);
		//printlist(m.tlist); //! only for testing
		m.clist = parser(m);
		//cmd = m.clist->value;
		//execute_single_builtins(&m, cmd);
		executor(m, cmd, envp);
		free_memory_for_next_line(&m);
	}
}
