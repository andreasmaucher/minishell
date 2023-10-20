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
int env(t_minishell *m)
{
    t_list *tmp;
    t_dict *dict;

    tmp = m->envp;
    dict = (t_dict *)tmp->value;
    while (tmp != NULL)
    {
        dict = (t_dict *)tmp->value;
        printf("%s\n", (char *)dict->value);
        printf("%s\n", dict->key);
        //printf("%s\n", m->env_lib[i]);  //! only test
        tmp = tmp->next;
    }
    return (0); //! exit_code
}