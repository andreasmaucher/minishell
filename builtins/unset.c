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
unset function is used to unset or remove environment variables or
shell variables
Syntax: 'unset variable_name'
Exit Status: The unset command typically returns a status of 0 if the variable
is successfully unset. If the variable does not exist, it still returns 0.
If multiple variable names are entered after unset, all will be checked and
unset even if incorrect variable names are included.
*/
int unset(t_minishell m, t_command *cmd)
{
    int i;
    int j;

    i = 1;
    while(cmd->args[i] != NULL)
    {
        if (check_if_part_of_library(m.env_lib, cmd->args[i]) == true)
        {
            while (m.env_lib[j] != NULL)
	        {
		        if (ft_strcmp(m.env_lib[j], cmd->args[i]) == 0)
			        free(m.env_lib[j]); //frees if the unset value was found
		        j++;
	        }
            i = -1; // go one back to be at the correct index
            if (m.envp_lib[j] != NULL)
                free(m.envp_lib[j]);
        }
        i++;
    }
    return (0); //! exit code
}