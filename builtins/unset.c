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
checks if the current key matches with the key in the user input;
if true the corresponding node is deleted and free 
*/
bool    check_for_key(t_minishell *m, t_command *cmd, int i, t_list *tmp)
{
    t_dict *dict;

    dict = (t_dict *)tmp->value;
    printf("DICT KEY %s\n", dict->key);
    printf("CMD->ARGS %s\n", cmd->args[i]);
    if (ft_strcmp(dict->key, cmd->args[i]) == 0)
    {
        ft_lstremove(&m->envp, tmp, del_envp);
        return (true);
    }
    return (false);
}

/*
unset function is used to unset or remove environment variables or
shell variables
Syntax: 'unset variable_name'
Exit Status: The unset command typically returns a status of 0 if the variable
is successfully unset. If the variable does not exist, it still returns 0.
If multiple variable names are entered after unset, all will be checked and
unset even if incorrect variable names are included.
*/
int unset(t_minishell *m, t_command *cmd)
{
    int i;
    t_list *tmp;

    i = 0;
    tmp = m->envp;
    while(cmd->args[i] != NULL)
    {
        tmp = m->envp;
        if (check_if_part_of_library(m->envp, cmd->args[i]) == true)
        {
            while (tmp != NULL)
	        {
                if (check_for_key(m, cmd, i, tmp) == true)
                    break;
                tmp = tmp->next;
	        }
        }
        i++;
    }
    return (0); //! exit code
}