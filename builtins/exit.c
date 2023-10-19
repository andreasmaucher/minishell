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

//DONE

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
- (if ac == 1) -> case for only 'echo'
*/

int exit_builtin(t_minishell m, t_command *cmd)
{
    int ac;
    int i;

    ac = arg_count(cmd->args);
    i = 1; // start after echo
    if (ac == 1) //case for only echo
    {
        global_exit_code = 0;
        exit_shell(m); //! or should this go to execution?
    }
    else if (ac != 1) // case for min. one more arg next to echo
    {
        while (cmd->args[i] != NULL)
        {
            if (ac == 2 && check_if_str_is_numeric(cmd->args[i]) == true) // case for echo + int
            {
                printf("exit");
                return(global_exit_code = 1);
            }
            else if (ac == 2 && check_if_str_is_numeric(cmd->args[i]) == false) // echo + word
            {
                printf("exit: %s: numeric argument required", cmd->args[1]);
                return(global_exit_code = 2);
            }
            else
            {
                printf("exit: too many arguments");
                return(global_exit_code = 1);
            }
            i++;
        }
    }
    return (0);
}