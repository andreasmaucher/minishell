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

//! when calling this function always first: cmd = (t_command *)clist->value;
//! this will ensure that we are accessing the args of the right command!
/* void    execute_builtins(t_minishell *m, t_command *cmd)
{
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        echo(m, cmd);
} */