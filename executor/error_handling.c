/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 04:09:15 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 04:10:25 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_handling_and_exit(char *error_msg)
{
	(void)error_msg;
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	exit(errno);
}
//alternative place for error msgs
//printf("Error code: %d, Error message: %s\n", errno, strerror(errno));

void	edge_cases(char *line)
{
	if (line[0] == '/')
		errno = 126;
	if (line[0] == '.' && line[1] == '/')
		errno = 126;
	if (line[0] == '.' && line[1] == ' ')
		errno = 2;
}
