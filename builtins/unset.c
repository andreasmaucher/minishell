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
unset function is used to unset or remove environment variables or
shell variables
Syntax: 'unset variable_name'
Exit Status: The unset command typically returns a status of 0 if the variable
is successfully unset. If the variable does not exist, it still returns 0.
*/

int unset(const char **argv, t_minishell m)
{
    int i,

    i = 0;
    while(argv[i] != NULL)
    {
        //!needs a function that removes env from my lib
    }
}