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

/*
returns the number of arguments within one command
*/
int arg_count(char **args)
{
    int i;

    i = 0;
    while (args[i] != NULL)
        i++;
    return(i);
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
*/

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

bool    check_if_str_is_numeric(char *arg_str)
{
    int i;

    i = 0;
    while (arg_str[i])
    {
        if (ft_isdigit(arg_str[i]) == 0)
            return(false);
        i++;
    }
    return(true);
}


int exit(t_minishell m, t_command *cmd)
{
    int ac;
    int i;

    ac = arg_count(cmd->args);
    i = 0;
    if (ac == 1)
    {
         exit_shell(m); //! or should this go to execution?
         g_exit_code = 0;
    }
    if (ac != 1)
    {
        while (cmd->args[i] != NULL)
        {
            if (check_if_str_is_numeric(cmd->args[i]) == false)
            {
                printf("exit: too many arguments");
                return(g_exit_code = 1);
            }
            i++;
        }
        else
        {
            printf("exit: %s: numeric argument required", cmd->args); //! DOES prob not work should be args[i]
        }
    }
    //! exit_shell
}