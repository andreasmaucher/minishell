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
Expected behavior:
- 'echo': prints the provided text to the terminal followed by a newline
- 'echo -n': prints the provided text to the terminal but suppresses newline, -n
needs to be the first word after echo, if not it's simply 'echoed'
*/

int echo(t_minishell m, t_command *cmd)
{
    bool     n_flag;
    int     i;
    t_command   *cmd;

    i = 1; // we want to start after echo
    n_flag = false;
    if (ft_strcmp(cmd->args[i], "-n") == 0)
    {
        n_flag = true;
        i = 2; // 0 for echo, 1 for -n, so 2 is first arg we want to check
    }
    while (cmd->args[i] != NULL)
    {
        if (ft_strcmp(cmd->args[i], "~") == 0) // ~ can occur at any point after echo
            cmd->args[i] = find_path(m.envp_lib, cmd->args[i]); // to print home directory path, searched in lib
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1] != NULL)
            printf(" "); // prints space after each word
        i++;
    }
    if (n_flag == false)
        printf("\n"); // print new line if n_flag is not set
    return (0); //! exit code?
}