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

void	check_weird_input(char *line, int *i)
{
	check_for_dot(line, i);
	if (errno == 0)
		check_for_slash(line, i);
	if (errno == 0)
		check_for_dot_slash(line, i);
}

void	check_for_dot(char *line, int *i)
{
	while (line[*i] == '.' && line[*i])
		(*i)++;
	if (line[*i] == '\0')
	{
		errno = 2;
		g_exit_code = errno;
		printf("Error code: %d, Error message: %s\n", errno, strerror(errno));
	}
}

void	check_for_slash(char *line, int *i)
{
	while (line[*i] == '/' && line[*i])
		(*i)++;
	if (line[*i] == '\0')
	{
		errno = 126;
		g_exit_code = errno;
		printf("Error code: %d, Error message: %s\n", errno, strerror(errno));
	}
}

void	check_for_dot_slash(char *line, int *i)
{
	while (line[*i] == '.' && line[*i])
		(*i)++;
	while (line[*i] == '/' && line[*i])
		(*i)++;
	if (line[*i] == '\0')
	{
		errno = 126;
		g_exit_code = errno;
		printf("Error code: %d, Error message: %s\n", errno, strerror(errno));
	}
}
