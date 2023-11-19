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

#include "../minishell.h"

/*
Expected behavior:
- 'echo': prints the provided text to the terminal followed by a newline
- 'echo -n': prints the provided text to the terminal but suppresses newline, -n
needs to be the first word after echo, if not it's simply 'echoed';
- i starts at 1 to skip the actual command name and 2 in case -n flag is set;
- '~' can occur at any point after echo and prints the home directory path;
*/

int echo(t_minishell m, t_command *cmd)
{
    bool    n_flag;
    int     i;
	int		j;

	j = 0;
    i = 1;
    n_flag = false;
    if (cmd->args[i] != NULL)
    {
        if (cmd->args[1][j] == '-' && cmd->args[1][++j] == 'n')
        {
			while (cmd->args[1][j] == 'n' && cmd->args[1][j])
				j++;
			if (cmd->args[1][j] == '\0')
			{
				n_flag = true;
            	i = 2;
			}
        }
        while (cmd->args[i] != NULL)
        {
            if (ft_strcmp(cmd->args[i], "~") == 0)
                cmd->args[i] = find_path_after_key(m.envp, "HOME");
            printf("%s", cmd->args[i]);
            if (cmd->args[i + 1] != NULL)
                printf(" ");
            //free(cmd->args[i]);  ///echo ~ was giving leaks, MR added this line
            i++;
        }
    }
    if (n_flag == false || cmd->args[1] == NULL)
        printf("\n");
    return (42);
}
