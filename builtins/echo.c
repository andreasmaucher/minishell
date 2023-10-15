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
Expected behavior:
- 'echo': prints the provided text to the terminal followed by a newline
- 'echo -n': prints the provided text to the terminal but suppresses newline, -n
needs to be the first word after echo, if not it's simply 'echoed'
*/

bool check_for_n_flag(char *arg)
{
    int i,

    i = 0;
    if (arg[i] != '-')
        return (false);
    i++;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (false);
        i++;
    }
    return (true);
}


int echo(t_minishell m)
{
   // loop through my clist node after node
}