/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 00:06:24 by mrizakov          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	no_cmd(t_command *cmd, t_minishell *m)
{
	if (cmd->path == NULL)
	{
		free_m(m);
		free_pipes(m);
		error_handling_and_exit("No valid command to run\n");
	}
}

void	free_all_filenames(t_command *cmd)
{
	free_filename(cmd->out_redirects.file_name);
}
