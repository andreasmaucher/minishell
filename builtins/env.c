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
imitates the behavior of the command 'env' and prints the env library incl.
the relevant path for each variable
*/
int env(t_minishell m)
{
    int i;

    i = 0;
    while (m.envp_lib[i] != NULL)
    {
        printf("%s\n", m.envp_lib[i]);
        i++;
    }
    return (0); //! exit_code
}