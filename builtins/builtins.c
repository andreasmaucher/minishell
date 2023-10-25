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

int    execute_builtins(t_minishell *m, t_command *cmd)
{
    int exit_code;

    exit_code = 0;
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        exit_code = echo(*m, cmd);
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
        exit_code = pwd();
    else if (ft_strcmp(cmd->args[0], "unset") == 0)
        exit_code = unset(m, cmd);
    else if (ft_strcmp(cmd->args[0], "env") == 0)
        exit_code = env(m);
    else if (ft_strcmp(cmd->args[0], "export") == 0)
        exit_code = export(m, cmd);
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
        exit_code = cd(m, cmd);
    return(exit_code);
}