/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
returns the number of arguments within one command
*/
int	arg_count(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

bool	check_if_str_is_numeric(char *arg_str)
{
	int	i;

	i = 0;
	while (arg_str[i])
	{
		if (ft_isdigit(arg_str[i]) == 0)
			return (false);
		i++;
	}
	return (true);
}

int	exit_conditions(t_command *cmd, int ac, int i, t_minishell *m)
{
	if (ac == 2 && check_if_str_is_numeric(cmd->args[i]) == true)
	{
		printf("exit\n");
		m->status_code2 = 1;
		return (exit_shell(*m));
	}
	else if (ac == 2 && check_if_str_is_numeric(cmd->args[i]) == false)
	{
		printf("exit: %s: numeric argument required\n", cmd->args[1]);
		m->status_code2 = 2;
		return (exit_shell(*m));
	}
	else if (ac > 2 && check_if_str_is_numeric(cmd->args[i]) == true)
	{
		printf("exit: too many arguments\n");
		m->status_code2 = 0;
		return (m->status_code2);
	}
	else
	{
		printf("exit: too many arguments\n");
		m->status_code2 = 1;
		return (exit_shell(*m));
	}
}

/*
mimics the behavior of the 'exit' command;
if more than one argument is given it does not exit the shell, but prints a 
warning (exit_code 1) and 'exit';
a standalone 'exit' command will have exit code 0;
Example behvaior:
exit 1: prints exit, but does not actually exit
exit 1 2: 'exit: too many arguments'
exit hi: 'exit: hi: numeric argument required'
exit hi man: 'exit: too many arguments'

function starts at index one to skip the actual command name;
- (if ac == 1) -> case for only 'exit'
*/
int	exit_builtin(t_minishell *m, t_command *cmd)
{
	int	ac;
	int	i;

	ac = arg_count(cmd->args);
	i = 1;
	if (ac == 1)
	{
		m->status_code2 = 0;
		exit_shell(*m);
	}
	else if (ac != 1)
	{
		while (cmd->args[i] != NULL)
		{
			exit_conditions(cmd, ac, i, m);
			if (m->status_code2 == 0)
				break ;
			i++;
		}
	}
	return (0);
}
